#include "GamePak.h"
#include "NES.h"
#include <cstring>
#include "INESParser.h"
#include <cstring>

GamePak::GamePak(void)
{
	rom = NULL;
	vram = new Memory(0x2000);
	nameTableRam = new Memory(0x1000);
	sram = new Memory(0x2000);

	// Designar los mirros de las nametables
	nameTableRam->createMemoryPage(0x0000, 0x0400, nameTables[0]);
	nameTableRam->createMemoryPage(0x0400, 0x0400, nameTables[1]);
	nameTableRam->createMemoryPage(0x0000, 0x0400, nameTables[2]);
	nameTableRam->createMemoryPage(0x0400, 0x0400, nameTables[3]);
}

GamePak::~GamePak(void)
{
	if(rom!=NULL)
		delete rom;
	if(vram!=NULL)
		delete vram;
	if(nameTableRam!=NULL)
		delete nameTableRam;
	if(sram!=NULL)
		delete sram;
}

Memory* GamePak::getRom(){
	return rom;
}

Memory* GamePak::getVRam(){
	return vram;
}

Memory* GamePak::getNameTablesRam(){
	return nameTableRam;
}

Memory* GamePak::getNameTable(int table){
	return &nameTables[table];
}

Memory* GamePak::getSRam(){
	return sram;
}

void GamePak::createRom(uint32 size){
	freeRom();
	rom = new Memory(size);
}

void GamePak::createVRam(uint32 size){
	freeVRam();
	vram = new Memory(size);
}

//TODO: dividir este método en 2 (uno read y otro write)
ubyte GamePak::access(uint32 addr, ubyte value, MemoryAcessMode mode){
	if(mode == ACESS_MODE_WRITE && addr >= 0x8000 && mappernum == 7){
        makeRomBanks(0x8000, 0x8000, (value&7));
		nameTableRam->createMemoryPage(0x400 * ((value>>4)&1), 0x0400, nameTables[0]);
		nameTables[1] = nameTables[2] = nameTables[3] = nameTables[0];
    }else if(mode == ACESS_MODE_WRITE && addr >= 0x8000 && mappernum == 2){
        makeRomBanks(0x4000, 0x8000, value);
    }else if(mode == ACESS_MODE_WRITE && addr >= 0x8000 && mappernum == 3){
		value &= access(addr, 0, ACESS_MODE_READ); // Simula un conflicto de bus
        makeVRomBanks(0x2000, 0x0000, (value&3));
    }else if(mode == ACESS_MODE_WRITE && addr >= 0x8000 && mappernum == 1){
        static ubyte regs[4]={0x0C,0,0,0}, counter=0, cache=0;
        
		if(value & 0x80) { regs[0]=0x0C; goto configure; }
        cache |= (value&1) << counter;
        
		if(++counter == 5)
        {
            regs[ (addr>>13) & 3 ] = value = cache;
        configure:
            cache = counter = 0;
            static const ubyte sel[4][4] = { {0,0,0,0}, {1,1,1,1}, {0,1,0,1}, {0,0,1,1} };
            for(uint32 m=0; m<4; ++m)
				nameTableRam->createMemoryPage(0x400 * sel[regs[0]&3][m], 0x0400, nameTables[m]);
            
			makeVRomBanks(0x1000, 0x0000, ((regs[0]&16) ? regs[1] : ((regs[1]&~1)+0)));
            makeVRomBanks(0x1000, 0x1000, ((regs[0]&16) ? regs[2] : ((regs[1]&~1)+1)));
            switch( (regs[0]>>2)&3 )
            {
                case 0: case 1:
                    makeRomBanks(0x8000, 0x8000, (regs[3] & 0xE) / 2);
                    break;
                case 2:
                    makeRomBanks(0x4000, 0x8000, 0);
                    makeRomBanks(0x4000, 0xC000, (regs[3] & 0xF));
                    break;
                case 3:
                    makeRomBanks(0x4000, 0x8000, (regs[3] & 0xF));
                    makeRomBanks(0x4000, 0xC000, ~0);
                    break;
            }
        }
    }

	if( (addr >> 13) == 3 ) return sram->readByte(addr & 0x1FFF);
	return banks[ (addr / ROM_GRANULARITY) % ROM_PAGES] [addr % ROM_GRANULARITY];
}

void GamePak::makeRomBanks(uint32 size, uint32 baseaddr, uint32 index){
	for(uint32 v = rom->getSize() + index * size,
		p = baseaddr / ROM_GRANULARITY;
		p < (baseaddr + size) / ROM_GRANULARITY && p < ROM_PAGES;
		++p, v += ROM_GRANULARITY)
			banks[p] = rom->getRealPhysicalAddress(v % rom->getSize());
}

void GamePak::makeVRomBanks(uint32 size, uint32 baseaddr, uint32 index){
	for(uint32 v = vram->getSize() + index * size,
		p = baseaddr / VRAM_GRANULARITY;
		p < (baseaddr + size) / VRAM_GRANULARITY && p < VRAM_PAGES;
		++p, v += VRAM_GRANULARITY)
			vBanks[p] = vram->getRealPhysicalAddress(v % vram->getSize());
}

void GamePak::freeRom(){
	if(rom)
		delete rom;
	rom = NULL;
}

void GamePak::freeVRam(){
	if(vram)
		delete vram;
	vram = NULL;
}

void GamePak::initBanks(){
	makeVRomBanks(0x2000, 0x0000, 0);
    for(unsigned v=0; v<4; ++v) 
		makeRomBanks(0x4000, v*0x4000, v==3 ? -1 : 0);
}

void GamePak::loadFromFile(char *filename){
	using namespace std;

	ifstream file;
	char name[5]={0,0,0,0,0};
	RomParser *parser;

	file.open(filename, ios::binary | ios::in);

	if(file.fail())
		throw "El archivo de rom no ha sido encontrado";

	file.read(name, 4);

	if(name[0]=='N' && name[1]=='E' && name[2]=='S' && name[3]==0x1A)
		parser = new INESParser();
	else
		throw "formato de archivo desconocido";

	parser->parse(file, *this);
	file.close();
	initBanks();
}

