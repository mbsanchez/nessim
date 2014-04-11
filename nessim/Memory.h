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


#if !defined(_MEMORY_H)
#define _MEMORY_H

#include "types.h"
#include <fstream>

using namespace std;

class Memory {
public:
	// Constructor por defecto
	Memory();
	// Constructor de la clase
	Memory(uint32 size);
	// Destructor de la clase
	~Memory();
	// Lee un byte de la memoria en la dirección indicada por el parámetro
	ubyte readByte(uint32 address);
	// Lee un valor short de la memoria, desde la dirección especificada por el parámetro
	ushort readShort(uint32 address);
	// Lee un segmento de memoria y lo almacena en el parametro buffer
	void read(uint32 address, ubyte* buffer, uint32 size);
	// escribe un byte en la memoria en la dirección indicada por el parámetro
	void writeByte(uint32 address, ubyte value);
	// escribe un semento de bytes a memoria, en la dirección específica
	void write(uint32 address, ubyte* buffer, uint32 size);
	// escribe un valor short en la memoria
	void writeShort(uint32 address, ushort value);
	// retorna la longitud de la memoria
	uint32 getSize();
	// limpia un espacio de memoria
	void clear(uint32 address, uint32 size);
	//Combina dos byte en un solo numero short
	static ushort makeShort(ubyte low, ubyte high);
	// hace un espejo del by en addr, en sepaciones de step
	void makeMirrorOfAddress(uint32 addr, uint32 upperBoundAddr, uint32 step);
	// Retorna la dirección real que equivale a la dirección lógica de la memoria emulada
	ubyte* getRealPhysicalAddress(uint32 addr);
	// Crea una página de memoria en mem a partir de la dirección indicada 
	void createMemoryPage(uint32 addr, uint32 size, Memory &mem);
	// Carga un conjunto de datos desde un archivo en la dirección específica
	void loadFromFile(ifstream &file, uint32 begAddr, uint32 size);
private:
	ubyte *buffer;
	uint32 size;
	bool isBufferOwner;
};

#endif  //_MEMORY_H
