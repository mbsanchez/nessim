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


#if !defined(_NES_H)
#define _NES_H

#include "NESMemory.h"
#include "NESCPU.h"

class NES {
public:
	friend class NESCPU;
	// Constructor de la clase
	NES();
	// Destructor de objetos de la clase
	~NES();
	// Retorna un valor que indica que se está realizando un acceso al DMA de la memoria
	bool isDMAAccessing();
	// Retorna la memoria del CPU
	NESMemory* getCpuMemory();
	// Retorna la memoria de la PPU
	NESMemory* getPpuMemory();
private:
	bool dmaAcessing;
	/*Memoria para la cpu*/
	NESMemory *ppuMemory;
	/*Memoria para la PPU*/
	NESMemory *cpuMemory;
	/*El CPU del sistema*/
	NESCPU *cpu;
};

#endif  //_NES_H
