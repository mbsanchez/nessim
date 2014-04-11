#pragma once

#ifndef GAMEPAK_H
#define GAMEPAK_H

#include "types.h"
#include "Memory.h"
#include "NESCPU.h"
#include "NESPPU.h"

class NES;

class GamePak
{
	friend class NES;
	friend class NESCPU;
	friend class NESPPU;
public:
	static const uint32 VRAM_GRANULARITY = 0x0400;
	static const uint32 ROM_GRANULARITY  = 0x2000;
	static const uint32 VRAM_PAGES = 0x2000 / VRAM_GRANULARITY;
	static const uint32 ROM_PAGES = 0x10000 / ROM_GRANULARITY;
private:
	// El tipo de mapper usado por el juego
	ubyte mappernum;
	// las memorias rom (programa) y vram (video, pattern tables)
	Memory *rom, *vram;
	// Nametable y SRAM (SpriteRAM)
    Memory *nameTableRam, *sram;
	// Mirror de nametables
    Memory nameTables[4];
	// bancos de la rom
	ubyte *banks[ROM_PAGES];
	// bancos de la vram
    ubyte *vBanks[VRAM_PAGES];
public:
	// Contructor
	GamePak(void);
	// Destructor
	~GamePak(void);
	// Prepara el GamePak
	void loadFromFile(char *filename);
	// inicializa los bancos de rom y vram
	void initBanks();
	// Retorna la memoria rom
	Memory* getRom();
	// Retorna la memoria ram
	Memory* getVRam();
	// Retorna la memoria de nametables
	Memory* getNameTablesRam();
	// Retorna la SRAM
	Memory* getSRam();
	// Retorna la nametable específica
	Memory* getNameTable(int table);
	// Crea la memoria rom
	void createRom(uint32 size);
	// Crea la memoria vram
	void createVRam(uint32 size);
	// Lee o escribe en la memoria rom
	ubyte access(uint32 addr, ubyte value, MemoryAcessMode mode);
	// Set mappernumber
	void setMapperNumber(ubyte mapper){ mappernum = mapper; }
private:
	// Crea los bancos de la rom
	void makeRomBanks(uint32 size, uint32 baseaddr, uint32 index);
	// Crea los bancos de la vram
	void makeVRomBanks(uint32 size, uint32 baseaddr, uint32 index);
	// Libera la memoria rom
	void freeRom();
	// Libera la memoria vram
	void freeVRam();
};

#endif