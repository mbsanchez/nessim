#include "INESParser.h"


INESParser::INESParser(void)
{
}


INESParser::~INESParser(void)
{
}

void INESParser::parse(ifstream &file, GamePak &cart){
	char name[4];
	file.seekg(0, ios::beg);

	file.read(name, 4);
	if(!(name[0]=='N' && name[1]=='E' && name[2]=='S' && name[3]==0x1A))
		throw "Formato de archivo incompatible";

	uint32 rom16countSize = file.get() * 0x4000;
    uint32 vrom8countSize = file.get() * 0x2000;
    ubyte ctrlbyte   = file.get();
    ubyte mappernum  = (file.get() & 0xF0) | (ctrlbyte >> 4);
	// Desechar 8 bytes
	file.seekg(8, ios::cur);

	if(mappernum >= 0x40) 
		mappernum &= 15;
    
	cart.setMapperNumber(mappernum);

	//Crear la rom y la vram
	if(rom16countSize)
		cart.createRom(rom16countSize);
	if(vrom8countSize)
		cart.createVRam(rom16countSize);

	cart.getRom()->loadFromFile(file, 0, rom16countSize);
	cart.getVRam()->loadFromFile(file, 0, vrom8countSize);
}
