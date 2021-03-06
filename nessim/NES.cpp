/*************************************************************************
 *   Copyright (C) 2014 by Manuel B. S�nchez                             *
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


#include "NES.h"
#include "Memory.h"
#include "NESCPU.h"
#include <cstring>

NES::NES(Region::RegionType type, AbstractDisplay *display): region(type) {
	this->display = display;
	cpu = new NESCPU(this);
	ppu = new NESPPU(this);
	cartridge = NULL;
}

NES::~NES() {
	delete cpu;
}

NESCPU* NES::getCPU(){
	return cpu;
}

Region NES::getRegion(){
	return region;
}

void NES::runFrame(){
	for(;;) 
		cpu->runInstruction();
}

void NES::syncHardware(){
	cpu->opcodeCycles--;
	ppu->tick();
	// apu_step();
	// mapper_tick();
}

void NES::insertCartridge(GamePak *cart, bool removePrev){
	if(removePrev && cartridge)
			delete cartridge;
	
	cartridge = cart;
	ppu->reset();
	cpu->hardReset();
	ppu->regs.data = 0;
}

