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


#include "Memory.h"
#include <stdlib.h>
#include <string.h>

using namespace std;

Memory::Memory(){
	buffer = NULL;
	size = 0;
	isBufferOwner = true;
}

Memory::Memory(uint32 size) {
	this->size = size;
	buffer = new ubyte[size];
	isBufferOwner = true;

	if(buffer == NULL)
		throw "error: no se ha podido inicializar la memoria del emulador";

	memset(buffer, 0, size);
}

Memory::~Memory() {
	if(isBufferOwner && buffer)
		delete [] buffer;
}

ubyte Memory::readByte(uint32 address) {
	if(address >= size)
		throw "error: dirección de memoria inválida";
	return buffer[address];
}

ushort Memory::readShort(uint32 address) {
	if(address >= size || address + 1 >= size)
		throw "error: dirección de memoria inválida";

	return Memory::makeShort(buffer[address], buffer[address+1]);
}

void Memory::read(uint32 address, ubyte* buffer, uint32 size) {
	if(address >= this->size || address + size > this->size)
		throw "error: dirección de memoria inválida";

	memcpy(buffer, buffer+address, size);
}

void Memory::writeByte(uint32 address, ubyte value) {
	if(address >= size)
		throw "error: dirección de memoria inválida";
	buffer[address] = value;
}

void Memory::write(uint32 address, ubyte* buffer, uint32 size) {
	if(address >= this->size || address + size > this->size)
		throw "error: dirección de memoria inválida";

	memcpy(buffer+address, buffer , size);
}

void Memory::writeShort(uint32 address, ushort value) {
	if(address >= size || address + 1 >= size)
		throw "error: dirección de memoria inválida";

	buffer[address] = value & 0xFF;
	buffer[address+1] = value >> 8;
}

uint32 Memory::getSize() {
	return size;
}

ushort Memory::makeShort(ubyte low, ubyte high){
	return (ushort)(high << 8 | low);
}

void Memory::clear(uint32 address, uint32 size){
	if(address >= this->size || address + size > this->size)
		throw "error: dirección de memoria inválida";

	memset(buffer + address, 0, size);
}

void Memory::makeMirrorOfAddress(uint32 addr, uint32 upperBoundAddr, uint32 step){
	for(uint32 i=addr+step;i<upperBoundAddr;i+=step)
		buffer[i] = buffer[addr];
}

ubyte* Memory::getRealPhysicalAddress(uint32 addr){
	if(addr >= size)
		throw "error: dirección de memoria inválida";
	return buffer + addr;
}

void Memory::createMemoryPage(uint32 beginAddress, uint32 size, Memory &mem){
	if(beginAddress >= this->size || beginAddress + size > this->size)
		throw "error: dirección de memoria inválida";

	mem.buffer = buffer + beginAddress;
	mem.size = size;
	mem.isBufferOwner = false;
}

void Memory::loadFromFile(ifstream &file, uint32 begAddr, uint32 size){
	if(begAddr >= this->size || begAddr + size > this->size)
		throw "error: dirección de memoria inválida";

	file.read((char*)buffer + begAddr, size);
}

