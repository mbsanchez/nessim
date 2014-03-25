/*************************************************************************
 *   Copyright (C) 2014 by Manuel B. Sánchez                             *
 *   manuelbsan@hotmail.com                                              *
 *                                                                       *
 *	 This file is part of Nessim.                                       *
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
#include "NESMemory.h"
#include "NESCPU.h"

NES::NES() {
	cpuMemory = new NESMemory(NESCPU::MEMORY_SIZE);
	// TODO:: Inicializar la memoria de la ppu
	ppuMemory = 0;
}

NES::~NES() {
	delete [] cpuMemory;
}

bool NES::isDMAAccessing() {
	return dmaAcessing;
}

NESMemory* NES::getCpuMemory() {
	return cpuMemory;
}

NESMemory* NES::getPpuMemory() {
	return ppuMemory;
}

