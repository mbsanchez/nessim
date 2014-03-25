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


#include "NESMemory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NESMemory::NESMemory(uint32 size) {
	this->size = size;
	buffer = new ubyte[size];

	if(buffer == NULL)
		throw "error: no se ha podido inicializar la memoria del emulador";
}

NESMemory::~NESMemory() {
	if(buffer)
		delete [] buffer;
}

ubyte NESMemory::readByte(ushort address) {
	if(address >= size)
		throw "error: dirección de memoria inválida";
	return buffer[address];
}

ushort NESMemory::readShort(ushort address) {
	if(address >= size || address + 1 >= size)
		throw "error: dirección de memoria inválida";

	return NESMemory::makeShort(buffer[address], buffer[address+1]);
}

void NESMemory::read(ushort address, ubyte* buffer, ushort size) {
	if(address >= this->size || address + size >= this->size)
		throw "error: dirección de memoria inválida";

	memcpy(buffer, buffer+address, size);
}

void NESMemory::writeByte(ushort address, ubyte value) {
	if(address >= size)
		throw "error: dirección de memoria inválida";
	buffer[address] = value;
}

void NESMemory::write(ushort address, ubyte* buffer, ushort size) {
	if(address >= this->size || address + size >= this->size)
		throw "error: dirección de memoria inválida";

	memcpy(buffer+address, buffer , size);
}

void NESMemory::writeShort(ushort address, ushort value) {
	if(address >= size || address + 1 >= size)
		throw "error: dirección de memoria inválida";

	buffer[address] = value & 0xFF;
	buffer[address+1] = value >> 8;
}

ushort NESMemory::getSize() {
	return size;
}

ushort NESMemory::makeShort(ubyte low, ubyte high){
	return (ushort)(high << 8 | low);
}

void NESMemory::clear(ushort address, ushort size){
	if(address >= this->size || address + size >= this->size)
		throw "error: dirección de memoria inválida";

	memset(buffer + address, 0, size);
}

void NESMemory::makeMirrorOfAddress(ushort addr, ushort upperBoundAddr, ushort step){
	for(int i=addr+step;i<upperBoundAddr;i+=step)
		buffer[i] = buffer[addr];
}

