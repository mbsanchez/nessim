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


#if !defined(_NESPPU_H)
#define _NESPPU_H

#include "Memory.h"
#include "Sprite.h"

class NES;

class NESPPU {
	friend class NES;
	int ciclo;
	union{
		uint32 data;
		struct {
			ubyte sysctrl:8;	ubyte dispctrl:8;
			ubyte status:8;		ubyte OAMaddr:8;
		};
		struct {
			//sysctrl
			ubyte BaseNTA: 2;	ubyte Inc:1;	
			ubyte SPaddr:1;		ubyte BGaddr:1;
			ubyte SPsize:1;		ubyte SlaveFlag:1;
			ubyte NMIenabled:1;
			//dispctrl
			ubyte Grayscale:1;	ubyte ShowBG8:1;	
			ubyte ShowSP8:1;	ubyte ShowBG:1;	
			ubyte ShowSP:1;		ubyte EmpRGB:3;		
			//status
			ubyte :5;
			ubyte SPoverflow:1;	ubyte SP0hit:1;
			ubyte InVBlank:1;
			//OAMaddr
			ubyte OAMdata:2;	ubyte OAMindex:6;
		};
		struct {
			ubyte :8, :3;
			ubyte ShowBGSP:2;
		};
	} regs;

	union
    {
		uint32 data;
		struct{
			uint32 :3;
			uint32 raw:16;
		};

        uint32 xscroll:8;
		struct {
			uint32 xfine:3;
			uint32 xcoarse:5;
			uint32 ycoarse:5;
			uint32 basenta:2;
			uint32 yfine:3;     
		};
		struct {
			uint32 :13;
			uint32 basenta_h:1;
			uint32 basenta_v:1;
		};
		struct{
			uint32 :3;
			uint32 vaddrlo:8;
			uint32 vaddrhi:8;
		};
    } scroll, vaddr;

public:
	// Longitud de la memoria de la PPU
	static const ushort MEMORY_SIZE = 0x4000;
	// Dirección donde se ubica la tabla de patrones 0 en la memoria
	static const ushort PATTERN_TABLE0_ADDRESS = 0;
	// Dirección donde se ubica la tabla de patrones 1 en la memoria
	static const ushort PATTERN_TABLE1_ADDRESS = 0x1000;
	// Dirección donde se ubica el nombre de la tabla 0 en la memoria.
	static const ushort NAME_TABLE0_ADDRESS = 0x2000;
	// Dirección donde se ubican los atributos de la tabla 0 en la memoria.
	static const ushort ATTRIBUTE_TABLE0_ADDRESS = 0x23C0;
	// Dirección donde se ubica el nombre de la tabla 1 en la memoria. Puede ser un espejo de otra tabla
	static const ushort NAME_TABLE1_ADDRESS = 0x2400;
	// Dirección donde se ubican los atributos de la tabla 1 en la memoria. Puede ser un espejo de otra tabla
	static const ushort ATTRIBUTE_TABLE1_ADDRESS = 0x27C0;
	// Dirección donde se ubica el nombre de la tabla 2 en la memoria. Puede ser un espejo de otra tabla
	static const ushort NAME_TABLE2_ADDRESS = 0x2800;
	// Dirección donde se ubican los atributos de la tabla 2 en la memoria. Puede ser un espejo de otra tabla
	static const ushort ATTRIBUTE_TABLE2_ADDRESS = 0x2BC0;
	// Dirección donde se ubica el nombre de la tabla 3 en la memoria. Puede ser un espejo de otra tabla
	static const ushort NAME_TABLE3_ADDRESS = 0x2C00;
	// Dirección donde se ubican los atributos de la tabla 3 en la memoria. Puede ser un espejo de otra tabla
	static const ushort ATTRIBUTE_TABLE3_ADDRESS = 0x2FC0;
	// Longitud del nombre de la tabla con sus atributos
	static const ushort NAME_TABLE_SIZE = 0x400;
	// Cantidad de bytes que serán usados en el espejo de este espacio de memoria
	static const ushort NAME_TABLES_MIRROR_SIZE = 0xF00;
	// Dirección donde se ubica el espejo de los nombres de las tablas en la memoria
	static const ushort NAME_TABLES_MIRROR_ADDRESS = 0x3000;
	// Dirección donde se ubica la paleta de colores de las imagenes en la memoria.
	static const ushort IMAGE_PALETTE_ADDRESS = 0x3F00;
	// Dirección donde se ubica la paleta de colores para los sprites en la memoria
	static const ushort SPRITE_PALETTE_ADRESS = 0x3F10;
	// Longitud del área de la paleta de colores en bytes
	static const ushort PALETTES_MIRROR_SIZE = 0x20;
	// Dirección de memoria donde deben ser respaldadas las paletas de colores
	static const ushort PALETTES_MIRROR_ADDRESS = 0x3F20;
	// Dirección de memoria donde termina el mirror de las paletas
	static const ushort PALETTES_MIRROR_END_ADDRESS = 0x3F40;
	// Constructor de la clase
	NESPPU(NES *nes);
	// Destructor de objetos de la clase
	~NESPPU();
	// Lee un byte de la memoria de la PPU
	ubyte readByteFromMemory(ushort address);
	// Escribe un byte a la memoria de la ppu
	void writeByteToMemory(ushort address, ubyte value);
	// Escribe la data del sprite a través de DMA
	void writeSpriteDMA(ubyte data);
	// Reiniciar el ppu
	void reset();
	// Tick
	void tick();
	// Step ppu
	void runStep();
	// Rendering step
	void renderingStep();
	// Render Pixel
	void renderPixel();
	// Lee información de la zona de memoria 0x2000 a 0x2007
	ubyte readFromIO(ushort addr);
	// Escribe información a la zona de memoria 0x2000 a 0x2007
	void writeToIO(ushort addr, ubyte value);
	// Incrementar registros 2007
	//void incrementDataReg();
	ubyte access(ushort addr, ubyte value, bool write);
private:
	//NES
	NES *nes;
	// Paletta
	Memory *palette;
	// Sprite data 256 bytes
	Memory *oamData;
	// Objetos usados para decodificar la información en cada scanline
	Sprite OAM2[8], OAM3[8];
	// Variable usada para ajustar los ciclos en PAL
	ubyte ticks;
	// Bus usado en la zona de registros IO
	ubyte openBus;

	uint32 pat_addr, sprinpos, sproutpos, sprrenpos, sprtmp;
    ushort tileattr, tilepat;
    uint32 bg_shift_pat, bg_shift_attr;

    int scanline, x, VBlankState, cycle_counter, scanline_end;
    int read_buffer, open_bus_decay_timer;
    bool even_odd_toggle, offset_toggle;

	uint32 ioAddress; //register[PPUADDR_REG]
};

#endif  //_NESPPU_H
