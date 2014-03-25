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


#if !defined(_NESMEMORY_H)
#define _NESMEMORY_H

#include "types.h"

class NESMemory {
public:
	// Constructor de la clase
	NESMemory(uint32 size);
	// Destructor de la clase
	~NESMemory();
	// Lee un byte de la memoria en la dirección indicada por el parámetro
	ubyte readByte(ushort address);
	// Lee un valor short de la memoria, desde la dirección especificada por el parámetro
	ushort readShort(ushort address);
	// Lee un segmento de memoria y lo almacena en el parametro buffer
	void read(ushort address, ubyte* buffer, ushort size);
	// escribe un byte en la memoria en la dirección indicada por el parámetro
	void writeByte(ushort address, ubyte value);
	// escribe un semento de bytes a memoria, en la dirección específica
	void write(ushort address, ubyte* buffer, ushort size);
	// escribe un valor short en la memoria
	void writeShort(ushort address, ushort value);
	// retorna la longitud de la memoria
	ushort getSize();
	// limpia un espacio de memoria
	void clear(ushort address, ushort size);
	//Combina dos byte en un solo numero short
	static ushort makeShort(ubyte low, ubyte high);
	// hace un espejo del by en addr, en sepaciones de step
	void makeMirrorOfAddress(ushort addr, ushort upperBoundAddr, ushort step);
private:
	ubyte *buffer;
	ushort size;
};

#endif  //_NESMEMORY_H
