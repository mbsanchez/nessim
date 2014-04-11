/*************************************************************************
 *   Copyright (C) 2014 by Manuel B. Sánchez                             *
 *   manuelbsan@hotmail.com                                              *
 *                                                                       *
 *	 This file is part of Nessim.                                        *
 *                                                                       *
 *   Nessim is free software: you can redistribute it and/or modify      *
 *   it under the terms of the GNU Lesser General Public License as      *
 *   published by the Free Software Foundation, either version 3 of      *
 *   the License, or (at your option) any later version.                 *
 *                                                                       *
 *   Nessim is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU Lesser General Public License for more details.                 *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.     *
 *************************************************************************/


#include "NESPPU.h"
#include "NES.h"
#include "IORegistersAddress.h"
#include <cstring>

NESPPU::NESPPU(NES *nes) {
	this->nes = nes;
	scroll.data = vaddr.data = 0;
	oamData = new Memory(0x100);
	palette = new Memory(PALETTES_MIRROR_SIZE);
	reset();
}

NESPPU::~NESPPU() {
	if(oamData)
		delete oamData;
	if(palette)
		delete palette;
}

void NESPPU::reset(){
	ticks = 0;
	ioAddress = 0;
	scroll.data = vaddr.data = 0;
	scanline=241, x=0, VBlankState=0, cycle_counter=0;
    read_buffer=0, open_bus_decay_timer=0;
	even_odd_toggle=false, offset_toggle=false;
	scanline_end = 314;
	openBus = 0;
	ciclo = 0;
	regs.data = 0;
	pat_addr = sprinpos = sproutpos = sprrenpos = sprtmp = 0;
    tileattr = tilepat = 0;
    bg_shift_pat =  bg_shift_attr = 0;
}

void NESPPU::tick(){
	if(nes->region.getType() == Region::REGION_PAL) {
		ticks++;
		if(ticks == 5) {
			ticks = 0;
			runStep();
		}
	}
	runStep();
	runStep();
	runStep();
}

void NESPPU::runStep(){
	// Set/clear vblank where needed
    switch(VBlankState)
    {
		case -5: regs.status = 0; break;
        case 2: 
			regs.status |= 0x80; break;
        case 0: 
			regs.InVBlank && regs.NMIenabled? nes->cpu->setNMI() : nes->cpu->clearNMI();
			break;
    }

    if(VBlankState != 0) 
		VBlankState += (VBlankState < 0 ? 1 : -1);

    if(open_bus_decay_timer) 
		if(!--open_bus_decay_timer) 
			openBus = 0;

    // Graphics processing scanline?
    if(scanline < 240)
    {
        /* Process graphics for this cycle */
		if(regs.ShowBGSP) 
			renderingStep();
        if(scanline >= 0 && x < 256) 
			renderPixel();
    }

    // Done with the cycle. Check for end of scanline.
    if(++cycle_counter == 3) 
		cycle_counter = 0; // For NTSC pixel shifting

    if(++x >= scanline_end)
    {
        // Begin new scanline
		nes->display->renderFrame(scanline);
        scanline_end = 341;
        x = 0;

        // Does something special happen on the new scanline?
        switch(scanline += 1)
        {
            case 261: // Begin of rendering
                scanline = -1; // pre-render line
                even_odd_toggle = !even_odd_toggle;
                // Clear vblank flag
                VBlankState = -5;
                break;
            case 241: // Begin of vertical blanking
                // I cheat here: I did not bother to learn how to use SDL events,
                // so I simply read button presses from a movie file, which happens
                // to be a TAS, rather than from the keyboard or from a joystick.
                /*static FILE* fp = fopen(inputfn, "rb");
                if(fp)
                {
                    static unsigned ctrlmask = 0;
                    if(!ftell(fp))
                    {
                        fseek(fp, 0x05, SEEK_SET);
                        ctrlmask = fgetc(fp);
                        fseek(fp, 0x90, SEEK_SET); // Famtasia Movie format.
                    }
                    if(ctrlmask & 0x80) { IO::joy_next[0] = fgetc(fp); if(feof(fp)) IO::joy_next[0] = 0; }
                    if(ctrlmask & 0x40) { IO::joy_next[1] = fgetc(fp); if(feof(fp)) IO::joy_next[1] = 0; }
                }*/
				switch(ciclo){
				case 5:
					nes->display->joyNext[0] = 0x40;
					break;
				case 6:
					nes->display->joyNext[0] = 0x0;
					break;
				case 10:
					nes->display->joyNext[0] = 0x40;
					break;
				case 11:
					nes->display->joyNext[0] = 0x0;
					break;
				case 15:
					nes->display->joyNext[0] = 0x40;
					break;
				case 16:
					nes->display->joyNext[0] = 0x0;
					break;
				case 20:
					nes->display->joyNext[0] = 0x40;
					break;
				case 21:
					nes->display->joyNext[0] = 0x0;
					break;
				/*case 14:
					nes->display->joyNext[0] = 0x80;
					break;
				case 15:
					nes->display->joyNext[0] = 0;
					break;
				case 16:
					nes->display->joyNext[0] = 0x80;
					break;
				case 17:
					nes->display->joyNext[0] = 0;
					break;
				case 30:
					nes->display->joyNext[0] = 0x80;
					break;
				case 31:
					nes->display->joyNext[0] = 0;
					break;
				case 50:
					nes->display->joyNext[0] = 0x80;
					break;
				case 51:
					nes->display->joyNext[0] = 0;
					break;*/
				}
                // Set vblank flag
                VBlankState = 2;
				ciclo++;
        }
    }
}

void NESPPU::renderingStep(){
	bool tile_decode_mode = (bool)(0x10FFFF & (1u << (x/16))); // When x is 0..255, 320..335

        // Each action happens in two steps: 1) select memory address; 2) receive data and react on it.
        switch(x % 8)
        {
            case 2: // Extraer la dirección de la tabla de atributos
				ioAddress = 0x23C0 + 0x400 * vaddr.basenta
							+ 8*(vaddr.ycoarse/4) + (vaddr.xcoarse/4);
				if(tile_decode_mode) 
					break; // Romper el case si tile_decode_mode es true
            case 0: // Extraer la información de la nametable
				ioAddress = 0x2000 + (vaddr.raw & 0xFFF);
                // Reiniciar la data del sprite
                if(x == 0) { 
					sprinpos = sproutpos = 0; 
					if(regs.ShowSP) 
						regs.OAMaddr = 0; 
				}

				if(!regs.ShowBG) 
					break;
                // reiniciar el desplazamiento vertical (vertical una vez, horizontal en cada scaline)
                if(x == 304 && scanline == -1) 
					vaddr.raw = scroll.raw;
                if(x == 256) { 
					vaddr.xcoarse = scroll.xcoarse;
					vaddr.basenta_h = scroll.basenta_h;
					sprrenpos = 0; 
				}
                break;
            case 1:
				if(x == 337 && scanline == -1 && even_odd_toggle && regs.ShowBG) 
					scanline_end = 340;
                // Acceso a la nametable
				pat_addr = 0x1000 * regs.BGaddr + 16 * 
						   readByteFromMemory(ioAddress) + vaddr.yfine;
                if(!tile_decode_mode) 
					break;
                // poner el tile actual en los registros shift
                // El bitmap es de 16 bits, los atributos 2 bits, repetido 8 veces
                bg_shift_pat  = (bg_shift_pat  >> 16) + 0x00010000 * tilepat;
                bg_shift_attr = (bg_shift_attr >> 16) + 0x55550000 * tileattr;
                break;
            case 3:
                // Attribute table access
                if(tile_decode_mode)
                {
					tileattr = (readByteFromMemory(ioAddress) >> 
							   ((vaddr.xcoarse & 2) + 2 * (vaddr.ycoarse & 2))) & 3;
                    // ir al siguiente tile horizontalmente (y activar nametable si es necesario)
					if(!++vaddr.xcoarse) { 
						vaddr.basenta_h = 1-vaddr.basenta_h; 
					}
					
                    // At the edge of the screen, do the same but vertically                 
					if(x==251){
						int x = vaddr.yfine + 1;
						vaddr.yfine = x;
						if (!vaddr.yfine){
							int y = vaddr.ycoarse + 1;
							vaddr.ycoarse = y;
							if(vaddr.ycoarse == 30){
								vaddr.ycoarse = 0; 
								vaddr.basenta_v = 1-vaddr.basenta_v; 
							}
						}
					}
                }else if(sprrenpos < sproutpos){
                    // Seleccionar el sprite pattern en vez del bg pattern
                    Sprite& o = OAM3[sprrenpos]; // sprite a renderizar en la próxima scanline
                    memcpy(&o, &OAM2[sprrenpos], sizeof(Sprite));
					uint32 y = (scanline) - o.getY();
                    if(o.getAttr() & 0x80) 
						y ^= (regs.SPsize ? 15 : 7);
					pat_addr = 0x1000 * (regs.SPsize ? (o.getIndex() & 0x01) : regs.SPaddr);
                    pat_addr +=  0x10 * (regs.SPsize ? (o.getIndex() & 0xFE) : (o.getIndex() & 0xFF));
                    pat_addr += (y & 7) + (y & 8) * 2;
                }
                break;
            // byte de la pattern table
            case 5:
				tilepat = readByteFromMemory(pat_addr | 0);
                break;
            case 7: // unir los bits de los dos pattern bytes
				unsigned p = tilepat | (readByteFromMemory(pat_addr | 8) << 8);
                p = (p & 0xF00F) | ((p & 0x0F00) >> 4) | ((p & 0x00F0) << 4);
                p = (p & 0xC3C3) | ((p & 0x3030) >> 2) | ((p & 0x0C0C) << 2);
                p = (p & 0x9999) | ((p & 0x4444) >> 1) | ((p & 0x2222) << 1);
                tilepat = p;
				// Cuando decodifique el sprite, guadar el grafico del sprite y mover a próximo
                if(!tile_decode_mode && sprrenpos < sproutpos)
                    OAM3[sprrenpos++].setPattern(tilepat);
                break;
        }
        // Buscar cual sprite es visible en la próxima scanline
		// (TODO: implement crazy 9-sprite malfunction)
		switch(x >= 64 && x < 256 && x % 2 ? (regs.OAMaddr++ & 3) : 4)
        {
            default:
                // Acceder a la OAM (object attribute memory)
				sprtmp = oamData->readByte(regs.OAMaddr);
                break;
            case 0:
                if(sprinpos >= 64) { 
					regs.OAMaddr=0; 
					break; 
				}
                ++sprinpos; // next sprite
                if(sproutpos<8) 
					OAM2[sproutpos].setY(sprtmp);
                if(sproutpos<8) 
					OAM2[sproutpos].setSprIndex(regs.OAMindex);
               {
				   int y1 = sprtmp, y2 = sprtmp + (regs.SPsize?16:8);
                
				   if(!( scanline >= y1 && scanline < y2 ))
					   regs.OAMaddr = sprinpos != 2 ? regs.OAMaddr + 3 : 8;
			   }
                break;
            case 1:
                if(sproutpos<8) 
					OAM2[sproutpos].setIndex(sprtmp);
                break;
            case 2:
                if(sproutpos<8) 
					OAM2[sproutpos].setAttr(sprtmp);
                break;
            case 3:
                if(sproutpos<8) 
					OAM2[sproutpos].setX(sprtmp);
                if(sproutpos<8) 
					++sproutpos; 
				else 
					regs.status |= 0x20;
                if(sprinpos == 2) 
					regs.OAMaddr = 8;
                break;
        }
}

void NESPPU::renderPixel(){
	bool edge   = ubyte(x+8) < 16; // 0..7, 248..255
	bool showbg = regs.ShowBG && (!edge || regs.ShowBG8);
	bool showsp = regs.ShowSP && (!edge || regs.ShowSP8);

    // Render the background
	unsigned fx = scroll.xfine, xpos = 15 - (((x & 7) + fx + 8*!!(x & 7) ) & 15);

    unsigned pixel = 0, attr = 0;
    
	if(showbg) // Pick a pixel from the shift registers
    {
        pixel = (bg_shift_pat  >> (xpos*2)) & 3;
        attr  = (bg_shift_attr >> (xpos*2)) & (pixel ? 3 : 0);
	}else if( (vaddr.raw & 0x3F00) == 0x3F00 && !regs.ShowBGSP)
		pixel = vaddr.raw;

    // Overlay the sprites
    if(showsp)
        for(unsigned sno=0; sno<sprrenpos; ++sno)
        {
            Sprite& s = OAM3[sno];
            // Check if this sprite is horizontally in range
			unsigned xdiff = x - s.getX();
            
			if(xdiff >= 8) 
				continue; // Also matches negative values
            
			// Determine which pixel to display; skip transparent pixels
            if(!(s.getAttr() & 0x40)) 
				xdiff = 7 - xdiff;
            ubyte spritepixel = (s.getPattern() >> (xdiff * 2)) & 3;

            if(!spritepixel) 
				continue;
            // Register sprite-0 hit if applicable
            if(x < 255 && pixel && s.getSprIndex() == 0) 
				regs.status |= 0x40;

            // Render the pixel unless behind-background placement wanted
            if(!(s.getAttr() & 0x20) || !pixel)
            {
                attr = (s.getAttr() & 3) + 4;
                pixel = spritepixel;
            }
            // Only process the first non-transparent sprite pixel.
            break;
        }
	pixel = palette->readByte((attr*4 + pixel) & 0x1F) & (regs.Grayscale ? 0x30 : 0x3F);
	nes->display->putPixel(x, scanline, pixel | (regs.EmpRGB << 6), cycle_counter);
}

//----------------------- Memoria de la PPU -------------------------------
ubyte NESPPU::readByteFromMemory(ushort address){
	/*if(address < NAME_TABLES_MIRROR_ADDRESS) //Pattern y name tables
		return nes->ppuMemory->readByte(address);
	else if(address < IMAGE_PALETTE_ADDRESS) // Mirror 2000-2EFF (nametables)
		return nes->ppuMemory->readByte(address-0x1000);
	else if(address < PALETTES_MIRROR_ADDRESS) // Paletas
		return nes->ppuMemory->readByte(address);
	else if(address < PALETTES_MIRROR_END_ADDRESS) // Mirror de paletas
		return nes->ppuMemory->readByte(address-PALETTES_MIRROR_SIZE);
	else if(address < MEMORY_SIZE) // Resto de la memoria
		return nes->ppuMemory->readByte(address);
	else // Mirror de la memoria de 0x4000 en adelante
		return readByteFromMemory(address & (MEMORY_SIZE-1));*/
	address &= 0x3FFF;
    if(address >= IMAGE_PALETTE_ADDRESS) { //Área de paletas
		if(address%4==0) 
			address &= 0x0F; 
		return palette->readByte(address & 0x1F); 
	}else if(address < NAME_TABLE0_ADDRESS) // Área de Pattern tables
		return nes->cartridge->vBanks[
			(address / GamePak::VRAM_GRANULARITY) % GamePak::VRAM_PAGES]
			[ address % GamePak::VRAM_GRANULARITY];
	// Área de nametables
	return nes->cartridge->nameTables[(address>>10) & 3].readByte(address & 0x3FF);
}

void NESPPU::writeByteToMemory(ushort address, ubyte value){
	/*if(address < NAME_TABLES_MIRROR_ADDRESS) //Pattern y name tables
		nes->ppuMemory->writeByte(address, value);
	else if(address < IMAGE_PALETTE_ADDRESS) // Mirror 2000-2EFF (nametables)
		nes->ppuMemory->writeByte(address-0x1000, value);
	else if(address < PALETTES_MIRROR_ADDRESS) // Paletas
		nes->ppuMemory->writeByte(address, value);
	else if(address < PALETTES_MIRROR_END_ADDRESS) // Mirror de paletas
		nes->ppuMemory->writeByte(address-PALETTES_MIRROR_SIZE, value);
	else if(address < MEMORY_SIZE) // Resto de la memoria
		nes->ppuMemory->writeByte(address, value);
	else // Mirror de la memoria de 0x4000 en adelante
		writeByteToMemory(address & (MEMORY_SIZE-1), value);*/
	address &= 0x3FFF;
    if(address >= IMAGE_PALETTE_ADDRESS) { //Área de paletas
		if(address%4==0) 
			address &= 0x0F; 
		palette->writeByte(address & 0x1F, value); 
	}else if(address < NAME_TABLE0_ADDRESS) // Área de Pattern tables
		nes->cartridge->vBanks[
						(address / GamePak::VRAM_GRANULARITY) % GamePak::VRAM_PAGES]
						[ address % GamePak::VRAM_GRANULARITY] = value;
	else // Área de nametables
		nes->cartridge->nameTables[(address>>10) & 3].writeByte(address & 0x3FF, value);
}
//----------------------- IO Regs --------------------------------------
void NESPPU::writeSpriteDMA(ubyte data){
	int i, k, tmp = data << 8;
	for (i = 0; i != 0x100; ++i)
	{
		//513 ciclos 1 for read, 1 for write, final is read
		k = nes->cpu->readByteFromMemory(tmp | i);
		nes->cpu->readByteFromMemory(tmp | i);
		oamData->writeByte((regs.OAMaddr + i) & 0xFF, k);
	}
	nes->cpu->readByteFromMemory(nes->cpu->getPC());
}

ubyte NESPPU::access(ushort addr, ubyte value, bool write){
	if(write){
		writeToIO(addr, value);
		return 0;
	}

	return readFromIO(addr);
}

void NESPPU::writeToIO(ushort addr, ubyte value){
	openBus = value, open_bus_decay_timer=77777;
	switch(addr & 7) {
		case 0:
			/*if(INVBLANK && (value & 0x80) && !NMI_ENABLE)
				nes->cpu->setNMI();
			if(((value & 0x80) == 0) && (scanline == nes->region.getVBlankStartLine()) && (linecycles < 4))
				nes->cpu->clearNMI();*/
			// tmpscroll:0xxxxABxxxxxxxxxx=value:xxxxxxAB
			// tmpscroll = (tmpscroll & 0x73FF) | ((value & 3) << 10);
			regs.sysctrl = value;
			scroll.basenta = regs.BaseNTA;
			break;
		case 1:
			//rendering = (value & 0x18) && (scanline < 240 || scanline == nes->region.getScreenEndLine());
			//registers[CTRL1_REG] = value;
			regs.dispctrl = value;
			break;
		case 3:
			regs.OAMaddr = value;
			break;
		case 4:
			//si se está renderizando
			//if(rendering)
			//	value = 0xFF;
			oamData->writeByte(regs.OAMaddr++, value);
			break;
		case 5: //scroll
			if(offset_toggle) { // segunda vez que se escribe
				// tmpscroll:xABCxxxxxxxxxxxx=value:xxxxxABC
				// tmpscroll &= 0x0C1F;
				// tmpscroll |= ((value & 0xF8) << 2) | ((value & 7) << 12);
				scroll.yfine   = value & 7; 
				scroll.ycoarse = value >> 3;
			} else { // primera vez que se escribe
				// scroll:xxxxxxxxxxxABCDE=value:ABCDExxx
				// scrollx=value:xxxxxABC
				// scroll = (tmpscroll & 0x7FE0) | (value >> 3);
				// scrollx = value & 7;
				scroll.xscroll = value;
			}
			offset_toggle = !offset_toggle;
			break;
		case 6: //vram addr
			if(offset_toggle) { // segunda escritura
				//scroll:xxxxxxxxABCDEFGH=value:ABCDEFGH
				//scroll = tmpscroll = (tmpscroll & 0x7F00) | value; 
				scroll.vaddrlo = value; 
				vaddr.raw = (uint32) scroll.raw;
			} else { // primera escritura
				//tmpscroll:xxABCDEFxxxxxxxx=value:xxABCDEF
				//tmpscroll:ABxxxxxxxxxxxxxx=0 (bits 14, 15 cleared)
				//tmpscroll = (tmpscroll & 0x00FF) | ((value & 0x7F) << 8);
				scroll.vaddrhi = value & 0x3F;
			}
			offset_toggle = !offset_toggle;
			break;	
		case 7:	//vram data
			writeByteToMemory(vaddr.raw, value);
			vaddr.raw = vaddr.raw + (regs.Inc ? 32 : 1);
			/*if((scroll & IMAGE_PALETTE_ADDRESS) != IMAGE_PALETTE_ADDRESS) {
				ioAddress = scroll & 0x3FFF;
				registers[PPUDATA_REG] = value;
				ioMode = 6;
			} else {
				addr = scroll;
				value &= 0x3F;
				writeByteToMemory(addr, value);
				//nes->videoScreen->updatePalette(addr, value);
				if((addr & 3) == 0)
					writeByteToMemory(IMAGE_PALETTE_ADDRESS+((addr & 0x1F) ^ 0x10), value);
			}
			incrementDataReg();*/

			break;
	}
}

ubyte NESPPU::readFromIO(ushort addr){
	ubyte ret = openBus;

	switch(addr & 7) {
		case 2:
			// Los 5 bits menores vienen desde open_bus
			ret = (regs.status) | (openBus & 0x1F);

			// Limpiar la bandera vblak del registro de status
			regs.InVBlank = 0;
			offset_toggle = false;

			if(VBlankState != -5)
				VBlankState = 0;
			// suprimir nmi
			/*if(scanline == nes->region.getVBlankStartLine()) {
				if(linecycles == 1) {
					ret &= 0x7F;
					nes->cpu->clearNMI();
				}
				if(linecycles < 4 && linecycles > 1) {
					nes->cpu->clearNMI();
				}
			}
			toggle = false;*/
			break;
		case 4:
			openBus = oamData->readByte(regs.OAMaddr);
			if(regs.OAMdata == 2)
				openBus &= 0xE3, open_bus_decay_timer = 77777;
			ret = openBus;
			break;
		case 7:
			ret = read_buffer;

			ubyte t = readByteFromMemory(vaddr.raw);
			if((vaddr.raw & IMAGE_PALETTE_ADDRESS) == IMAGE_PALETTE_ADDRESS){ //Paleta
				ret = (openBus & 0xC0) | (t & 0x3F);
				read_buffer = readByteFromMemory(vaddr.raw & 0x2FFF); 
			}else{
				ret = read_buffer;
				read_buffer = t;
			}
			openBus = ret, open_bus_decay_timer = 77777;
			vaddr.raw = vaddr.raw + (regs.Inc ? 32 : 1);
			/*//configurar IO de la ppu
			ioAddress = scroll & 0x3FFF;
			ioMode = 5;

			//incrementar los registros
			incrementDataReg();

			//lectura de paletas
			if(ioAddress >= IMAGE_PALETTE_ADDRESS) {
				openBus &= 0xC0;
				openBus |= readByteFromMemory(ioAddress);
			}else 
				openBus = latch;
			break;*/
	}

	return ret;
}
