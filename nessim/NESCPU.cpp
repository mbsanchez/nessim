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

#include "NESCPU.h"
#include "NES.h"
#include "AddressingMode.h"

#define CASE_OPCODE(n,o,a)			\
	case 0x##n:						\
		this->calculateAddress(a);	\
		this->execute##o();			\
		break;

const ubyte NESCPU::CYCLES_BY_OPCODE[] = {  0x07,	0x06,	0x00,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x03,	0x02,	0x02,	0x02,	0x04,	0x04,	0x06,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07,
											0x06,	0x06,	0x00,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x04,	0x02,	0x02,	0x02,	0x04,	0x04,	0x06,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07,
											0x06,	0x06,	0x00,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x03,	0x02,	0x02,	0x02,	0x03,	0x04,	0x06,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07,
											0x06,	0x06,	0x00,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x04,	0x02,	0x02,	0x02,	0x05,	0x04,	0x04,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07,
											0x02,	0x06,	0x02,	0x06,	0x03,	0x03,	0x03,	0x03,	
											0x02,	0x02,	0x02,	0x02,	0x04,	0x04,	0x04,	0x04,
											0x82,	0x06,	0x00,	0x06,	0x04,	0x04,	0x04,	0x04,	
											0x02,	0x05,	0x02,	0x05,	0x05,	0x05,	0x05,	0x05,
											0x02,	0x06,	0x02,	0x06,	0x03,	0x03,	0x03,	0x03,	
											0x02,	0x02,	0x02,	0x02,	0x04,	0x04,	0x04,	0x04,
											0x82,	0x85,	0x00,	0x85,	0x04,	0x04,	0x04,	0x04,	
											0x02,	0x84,	0x02,	0x84,	0x84,	0x84,	0x84,	0x84,
											0x02,	0x06,	0x02,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x02,	0x02,	0x02,	0x02,	0x04,	0x04,	0x06,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07,
											0x02,	0x06,	0x02,	0x08,	0x03,	0x03,	0x05,	0x05,	
											0x02,	0x02,	0x02,	0x02,	0x04,	0x04,	0x06,	0x06,
											0x82,	0x85,	0x00,	0x08,	0x04,	0x04,	0x06,	0x06,	
											0x02,	0x84,	0x02,	0x07,	0x84,	0x84,	0x07,	0x07 };

NESCPU::NESCPU(NES *nes) {
	this->nes = nes;
	hardReset();
}

NESCPU::~NESCPU() {
}

void NESCPU::emulateFrame() {

}

void NESCPU::emulateInstruction() {
	operationAddress = PC;
	ubyte opcode = readByteFromMemory(PC++);

	switch(opcode){
		CASE_OPCODE(00,BRK, ADM_IMP)	CASE_OPCODE(01,ORA, ADM_INX)	
		CASE_OPCODE(02,UNK, ADM_UNK)	CASE_OPCODE(03,SLO, ADM_INX)	
		CASE_OPCODE(04,NOP, ADM_ZPG)	CASE_OPCODE(05,ORA, ADM_ZPG)	
		CASE_OPCODE(06,ASL, ADM_ZPG)	CASE_OPCODE(07,SLO, ADM_ZPG)
		CASE_OPCODE(08,PHP, ADM_IMP)	CASE_OPCODE(09,ORA, ADM_IMM)	
		CASE_OPCODE(0A,ASLA, ADM_IMP)	CASE_OPCODE(0B,AAC, ADM_IMM)	
		CASE_OPCODE(0C,NOP, ADM_ABS)	CASE_OPCODE(0D,ORA, ADM_ABS)	
		CASE_OPCODE(0E,ASL, ADM_ABS)	CASE_OPCODE(0F,SLO, ADM_ABS)
		CASE_OPCODE(10,BPL, ADM_REL)	CASE_OPCODE(11,ORA, ADM_INY)	
		CASE_OPCODE(12,UNK, ADM_UNK)	CASE_OPCODE(13,SLO, ADM_INY)	
		CASE_OPCODE(14,NOP, ADM_ZPX)	CASE_OPCODE(15,ORA, ADM_ZPX)	
		CASE_OPCODE(16,ASL, ADM_ZPX)	CASE_OPCODE(17,SLO, ADM_ZPX)
		CASE_OPCODE(18,CLC, ADM_IMP)	CASE_OPCODE(19,ORA, ADM_ABY)	
		CASE_OPCODE(1A,NOP, ADM_IMP)	CASE_OPCODE(1B,SLO, ADM_ABY)	
		CASE_OPCODE(1C,NOP, ADM_ABX)	CASE_OPCODE(1D,ORA, ADM_ABX)	
		CASE_OPCODE(1E,ASL, ADM_ABX)	CASE_OPCODE(1F,SLO, ADM_ABX)
		CASE_OPCODE(20,JSR, ADM_ABS)	CASE_OPCODE(21,AND, ADM_INX)	
		CASE_OPCODE(22,UNK, ADM_UNK)	CASE_OPCODE(23,RLA, ADM_INX)	
		CASE_OPCODE(24,BIT, ADM_ZPG)	CASE_OPCODE(25,AND, ADM_ZPG)	
		CASE_OPCODE(26,ROL, ADM_ZPG)	CASE_OPCODE(27,RLA, ADM_ZPG)
		CASE_OPCODE(28,PLP, ADM_IMP)	CASE_OPCODE(29,AND, ADM_IMM)	
		CASE_OPCODE(2A,ROLA, ADM_IMP)	CASE_OPCODE(2B,AAC, ADM_IMM)	
		CASE_OPCODE(2C,BIT, ADM_ABS)	CASE_OPCODE(2D,AND, ADM_ABS)	
		CASE_OPCODE(2E,ROL, ADM_ABS)	CASE_OPCODE(2F,RLA, ADM_ABS)
		CASE_OPCODE(30,BMI, ADM_REL)	CASE_OPCODE(31,AND, ADM_INY)	
		CASE_OPCODE(32,UNK, ADM_UNK)	CASE_OPCODE(33,RLA, ADM_INY)	
		CASE_OPCODE(34,NOP, ADM_ZPX)	CASE_OPCODE(35,AND, ADM_ZPX)	
		CASE_OPCODE(36,ROL, ADM_ZPX)	CASE_OPCODE(37,RLA, ADM_ZPX)
		CASE_OPCODE(38,SEC, ADM_IMP)	CASE_OPCODE(39,AND, ADM_ABY)	
		CASE_OPCODE(3A,NOP, ADM_IMP)	CASE_OPCODE(3B,RLA, ADM_ABY)	
		CASE_OPCODE(3C,NOP, ADM_ABX)	CASE_OPCODE(3D,AND, ADM_ABX)	
		CASE_OPCODE(3E,ROL, ADM_ABX)	CASE_OPCODE(3F,RLA, ADM_ABX)
		CASE_OPCODE(40,RTI, ADM_IMP)	CASE_OPCODE(41,EOR, ADM_INX)	
		CASE_OPCODE(42,UNK, ADM_UNK)	CASE_OPCODE(43,SRE, ADM_INX)	
		CASE_OPCODE(44,NOP, ADM_ZPG)	CASE_OPCODE(45,EOR, ADM_ZPG)	
		CASE_OPCODE(46,LSR, ADM_ZPG)	CASE_OPCODE(47,SRE, ADM_ZPG)
		CASE_OPCODE(48,PHA, ADM_IMP)	CASE_OPCODE(49,EOR, ADM_IMM)	
		CASE_OPCODE(4A,LSRA, ADM_IMP)	CASE_OPCODE(4B,ASR, ADM_IMM)	
		CASE_OPCODE(4C,JMP, ADM_ABS)	CASE_OPCODE(4D,EOR, ADM_ABS)	
		CASE_OPCODE(4E,LSR, ADM_ABS)	CASE_OPCODE(4F,SRE, ADM_ABS)
		CASE_OPCODE(50,BVC, ADM_REL)	CASE_OPCODE(51,EOR, ADM_INY)	
		CASE_OPCODE(52,UNK, ADM_UNK)	CASE_OPCODE(53,SRE, ADM_INY)	
		CASE_OPCODE(54,NOP, ADM_ZPX)	CASE_OPCODE(55,EOR, ADM_ZPX)	
		CASE_OPCODE(56,LSR, ADM_ZPX)	CASE_OPCODE(57,SRE, ADM_ZPX)
		CASE_OPCODE(58,CLI, ADM_IMP)	CASE_OPCODE(59,EOR, ADM_ABY)	
		CASE_OPCODE(5A,NOP, ADM_IMP)	CASE_OPCODE(5B,SRE, ADM_ABY)	
		CASE_OPCODE(5C,NOP, ADM_ABX)	CASE_OPCODE(5D,EOR, ADM_ABX)	
		CASE_OPCODE(5E,LSR, ADM_ABX)	CASE_OPCODE(5F,SRE, ADM_ABX)
		CASE_OPCODE(60,RTS, ADM_IMP)	CASE_OPCODE(61,ADC, ADM_INX)	
		CASE_OPCODE(62,UNK, ADM_UNK)	CASE_OPCODE(63,RRA, ADM_INX)	
		CASE_OPCODE(64,NOP, ADM_ZPG)	CASE_OPCODE(65,ADC, ADM_ZPG)	
		CASE_OPCODE(66,ROR, ADM_ZPG)	CASE_OPCODE(67,RRA, ADM_ZPG)
		CASE_OPCODE(68,PLA, ADM_IMP)	CASE_OPCODE(69,ADC, ADM_IMM)	
		CASE_OPCODE(6A,RORA, ADM_IMP)	CASE_OPCODE(6B,ARR, ADM_IMM)	
		CASE_OPCODE(6C,JMP, ADM_IND)	CASE_OPCODE(6D,ADC, ADM_ABS)	
		CASE_OPCODE(6E,ROR, ADM_ABS)	CASE_OPCODE(6F,RRA, ADM_ABS)
		CASE_OPCODE(70,BVS, ADM_REL)	CASE_OPCODE(71,ADC, ADM_INY)	
		CASE_OPCODE(72,UNK, ADM_UNK)	CASE_OPCODE(73,RRA, ADM_INY)	
		CASE_OPCODE(74,NOP, ADM_ZPX)	CASE_OPCODE(75,ADC, ADM_ZPX)	
		CASE_OPCODE(76,ROR, ADM_ZPX)	CASE_OPCODE(77,RRA, ADM_ZPX)
		CASE_OPCODE(78,SEI, ADM_IMP)	CASE_OPCODE(79,ADC, ADM_ABY)	
		CASE_OPCODE(7A,NOP, ADM_IMP)	CASE_OPCODE(7B,RRA, ADM_ABY)	
		CASE_OPCODE(7C,NOP, ADM_ABX)	CASE_OPCODE(7D,ADC, ADM_ABX)	
		CASE_OPCODE(7E,ROR, ADM_ABX)	CASE_OPCODE(7F,RRA, ADM_ABX)
		CASE_OPCODE(80,NOP, ADM_IMM)	CASE_OPCODE(81,STA, ADM_INX)	
		CASE_OPCODE(82,NOP, ADM_IMM)	CASE_OPCODE(83,SAX, ADM_INX)	
		CASE_OPCODE(84,STY, ADM_ZPG)	CASE_OPCODE(85,STA, ADM_ZPG)	
		CASE_OPCODE(86,STX, ADM_ZPG)	CASE_OPCODE(87,SAX, ADM_ZPG)
		CASE_OPCODE(88,DEY, ADM_IMP)	CASE_OPCODE(89,NOP, ADM_IMM)	
		CASE_OPCODE(8A,TXA, ADM_IMP)	CASE_OPCODE(8B,XAA, ADM_IMM)	
		CASE_OPCODE(8C,STY, ADM_ABS)	CASE_OPCODE(8D,STA, ADM_ABS)	
		CASE_OPCODE(8E,STX, ADM_ABS)	CASE_OPCODE(8F,SAX, ADM_ABS)
		CASE_OPCODE(90,BCC, ADM_REL)	CASE_OPCODE(91,STA, ADM_INY)	
		CASE_OPCODE(92,UNK, ADM_UNK)	CASE_OPCODE(93,AXA, ADM_INY)	
		CASE_OPCODE(94,STY, ADM_ZPX)	CASE_OPCODE(95,STA, ADM_ZPX)	
		CASE_OPCODE(96,STX, ADM_ZPY)	CASE_OPCODE(97,SAX, ADM_ZPY)
		CASE_OPCODE(98,TYA, ADM_IMP)	CASE_OPCODE(99,STA, ADM_ABY)	
		CASE_OPCODE(9A,TXS, ADM_IMP)	CASE_OPCODE(9B,XAS, ADM_ABY)	
		CASE_OPCODE(9C,SYA, ADM_ABX)	CASE_OPCODE(9D,STA, ADM_ABX)	
		CASE_OPCODE(9E,SXA, ADM_ABY)	CASE_OPCODE(9F,AXA, ADM_ABY)
		CASE_OPCODE(A0,LDY, ADM_IMM)	CASE_OPCODE(A1,LDA, ADM_INX)	
		CASE_OPCODE(A2,LDX, ADM_IMM)	CASE_OPCODE(A3,LAX, ADM_INX)	
		CASE_OPCODE(A4,LDY, ADM_ZPG)	CASE_OPCODE(A5,LDA, ADM_ZPG)	
		CASE_OPCODE(A6,LDX, ADM_ZPG)	CASE_OPCODE(A7,LAX, ADM_ZPG)
		CASE_OPCODE(A8,TAY, ADM_IMP)	CASE_OPCODE(A9,LDA, ADM_IMM)	
		CASE_OPCODE(AA,TAX, ADM_IMP)	CASE_OPCODE(AB,ATX, ADM_IMM)	
		CASE_OPCODE(AC,LDY, ADM_ABS)	CASE_OPCODE(AD,LDA, ADM_ABS)	
		CASE_OPCODE(AE,LDX, ADM_ABS)	CASE_OPCODE(AF,LAX, ADM_ABS)
		CASE_OPCODE(B0,BCS, ADM_REL)	CASE_OPCODE(B1,LDA, ADM_INY)	
		CASE_OPCODE(B2,UNK, ADM_UNK)	CASE_OPCODE(B3,LAX, ADM_INY)	
		CASE_OPCODE(B4,LDY, ADM_ZPX)	CASE_OPCODE(B5,LDA, ADM_ZPX)	
		CASE_OPCODE(B6,LDX, ADM_ZPY)	CASE_OPCODE(B7,LAX, ADM_ZPY)
		CASE_OPCODE(B8,CLV, ADM_IMP)	CASE_OPCODE(B9,LDA, ADM_ABY)	
		CASE_OPCODE(BA,TSX, ADM_IMP)	CASE_OPCODE(BB,LAR, ADM_ABY)	
		CASE_OPCODE(BC,LDY, ADM_ABX)	CASE_OPCODE(BD,LDA, ADM_ABX)	
		CASE_OPCODE(BE,LDX, ADM_ABY)	CASE_OPCODE(BF,LAX, ADM_ABY)
		CASE_OPCODE(C0,CPY, ADM_IMM)	CASE_OPCODE(C1,CMP, ADM_INX)	
		CASE_OPCODE(C2,NOP, ADM_IMM)	CASE_OPCODE(C3,DCP, ADM_INX)	
		CASE_OPCODE(C4,CPY, ADM_ZPG)	CASE_OPCODE(C5,CMP, ADM_ZPG)	
		CASE_OPCODE(C6,DEC, ADM_ZPG)	CASE_OPCODE(C7,DCP, ADM_ZPG)
		CASE_OPCODE(C8,INY, ADM_IMP)	CASE_OPCODE(C9,CMP, ADM_IMM)	
		CASE_OPCODE(CA,DEX, ADM_IMP)	CASE_OPCODE(CB,AXS, ADM_IMM)	
		CASE_OPCODE(CC,CPY, ADM_ABS)	CASE_OPCODE(CD,CMP, ADM_ABS)	
		CASE_OPCODE(CE,DEC, ADM_ABS)	CASE_OPCODE(CF,DCP, ADM_ABS)
		CASE_OPCODE(D0,BNE, ADM_REL)	CASE_OPCODE(D1,CMP, ADM_INY)	
		CASE_OPCODE(D2,UNK, ADM_UNK)	CASE_OPCODE(D3,DCP, ADM_INY)	
		CASE_OPCODE(D4,NOP, ADM_ZPX)	CASE_OPCODE(D5,CMP, ADM_ZPX)	
		CASE_OPCODE(D6,DEC, ADM_ZPX)	CASE_OPCODE(D7,DCP, ADM_ZPX)
		CASE_OPCODE(D8,CLD, ADM_IMP)	CASE_OPCODE(D9,CMP, ADM_ABY)	
		CASE_OPCODE(DA,NOP, ADM_IMP)	CASE_OPCODE(DB,DCP, ADM_ABY)	
		CASE_OPCODE(DC,NOP, ADM_ABX)	CASE_OPCODE(DD,CMP, ADM_ABX)	
		CASE_OPCODE(DE,DEC, ADM_ABX)	CASE_OPCODE(DF,DCP, ADM_ABX)
		CASE_OPCODE(E0,CPX, ADM_IMM)	CASE_OPCODE(E1,SBC, ADM_INX)	
		CASE_OPCODE(E2,NOP, ADM_IMM)	CASE_OPCODE(E3,ISB, ADM_INX)	
		CASE_OPCODE(E4,CPX, ADM_ZPG)	CASE_OPCODE(E5,SBC, ADM_ZPG)	
		CASE_OPCODE(E6,INC, ADM_ZPG)	CASE_OPCODE(E7,ISB, ADM_ZPG)
		CASE_OPCODE(E8,INX, ADM_IMP)	CASE_OPCODE(E9,SBC, ADM_IMM)	
		CASE_OPCODE(EA,NOP, ADM_IMP)	CASE_OPCODE(EB,SBC, ADM_IMM)	
		CASE_OPCODE(EC,CPX, ADM_ABS)	CASE_OPCODE(ED,SBC, ADM_ABS)	
		CASE_OPCODE(EE,INC, ADM_ABS)	CASE_OPCODE(EF,ISB, ADM_ABS)
		CASE_OPCODE(F0,BEQ, ADM_REL)	CASE_OPCODE(F1,SBC, ADM_INY)	
		CASE_OPCODE(F2,UNK, ADM_UNK)	CASE_OPCODE(F3,ISB, ADM_INY)	
		CASE_OPCODE(F4,NOP, ADM_ZPX)	CASE_OPCODE(F5,SBC, ADM_ZPX)	
		CASE_OPCODE(F6,INC, ADM_ZPX)	CASE_OPCODE(F7,ISB, ADM_ZPX)
		CASE_OPCODE(F8,SED, ADM_IMP)	CASE_OPCODE(F9,SBC, ADM_ABY)	
		CASE_OPCODE(FA,NOP, ADM_IMP)	CASE_OPCODE(FB,ISB, ADM_ABY)	
		CASE_OPCODE(FC,NOP, ADM_ABX)	CASE_OPCODE(FD,SBC, ADM_ABX)	
		CASE_OPCODE(FE,INC, ADM_ABX)	CASE_OPCODE(FF,ISB, ADM_ABX)
	}
}

void NESCPU::softReset() {
	PC = readShortFromMemory(RESET_HANDLER_ADDRESS);
	P |= INTERRUPT_DISABLE_FLAG;
}

void NESCPU::hardReset() {
	A = X = Y = 0;
	P = 0x24 | ZERO_FLAG;
	SP = SP_BEGIN;
	cycles = 0;
	effectiveAddress = 0;
	PC = readShortFromMemory(RESET_HANDLER_ADDRESS);
	nes->getCpuMemory()->clear(ZERO_PAGE_ADRESS, RAM_SIZE*4);
	nmi = prevNmi = irq = prevIrq = 0;
}

void NESCPU::tick(){
	cycles++;
}

ushort NESCPU::getPC() {
	return PC;
}

ushort NESCPU::getSP() {
	return SP;
}

ushort NESCPU::getA() {
	return A;
}

ushort NESCPU::getX() {
	return X;
}

ushort NESCPU::getY() {
	return Y;
}

ubyte NESCPU::getP() {
	return P;
}

uint64 NESCPU::getCycles(){
	return cycles;
}

void NESCPU::writeByteToMemory(ushort address, byte value){
	nes->cpuMemory->writeByte(address, value);

	if(address < RAM_SIZE){
		nes->cpuMemory->makeMirrorOfAddress(address, IO_REGISTERS_ADDRESS, RAM_SIZE);
	}else if(address >= IO_REGISTERS_ADDRESS && address < IO_REGISTERS_ADDRESS+IO_REGISTERS_SIZE){
		nes->cpuMemory->makeMirrorOfAddress(address, 0x4000, IO_REGISTERS_SIZE);
	}
}

ubyte NESCPU::readByteFromMemory(ushort address){
	return nes->getCpuMemory()->readByte(address);
}

ushort NESCPU::readShortFromMemory(ushort address){
	return nes->getCpuMemory()->readShort(address);
}

void NESCPU::calculateAddress(AddressingMode mode){
	switch(mode){
		case ADM_UNK:
			throw "error: modo de direccionamiento no soportado";
			break;
		case ADM_IMP: // No se usa dirección
			effectiveAddress = -1;
			break;
		case ADM_IMM: // La dirección del valor es el PC
			effectiveAddress = PC++;
			break;
		case ADM_ABS: 
			effectiveAddress = readShortFromMemory(PC);
			PC+=2;
			break;
		/*case ADM_AXR:
			effectiveAddress = readShortFromMemory(PC) + X;
			PC+=2;
			break;*/
		case ADM_ABX:
			effectiveAddress = readShortFromMemory(PC) + X;
			PC+=2;
			break;
		/*case ADM_AYR:
			effectiveAddress = readShortFromMemory(PC) + Y;
			PC+=2;
			break;*/
		case ADM_ABY:
			effectiveAddress = readShortFromMemory(PC) + Y;
			PC+=2;
			break;
		case ADM_ZPG:
			effectiveAddress = readByteFromMemory(PC++);
			break;
		case ADM_ZPX:
			effectiveAddress = (readByteFromMemory(PC++) + X) & 0xFF;
			break;
		case ADM_ZPY:
			effectiveAddress = (readByteFromMemory(PC++) + Y) & 0xFF;
			break;
		case ADM_INX:
			effectiveAddress = readByteFromMemory(PC++);
			effectiveAddress = readShortFromMemory(effectiveAddress)+X;
			break;
		/*case ADM_IYR:
			effectiveAddress = readByteFromMemory(PC++);
			effectiveAddress = readShortFromMemory(effectiveAddress)+Y;
			break;*/
		case ADM_INY:
			effectiveAddress = readByteFromMemory(PC++);
			effectiveAddress = readShortFromMemory(effectiveAddress)+Y;
			break;
		case ADM_IND:
			effectiveAddress = readShortFromMemory(PC);
			effectiveAddress = readShortFromMemory(effectiveAddress);
			PC+=2;
			break;
		case ADM_REL:
			byte operando = readByteFromMemory(PC++);
			effectiveAddress = PC + operando;
			break;
	}
}

void NESCPU::toggleNegativeOrZeroFlag(byte value){
	P = (value==0)? P | ZERO_FLAG: P & ~ZERO_FLAG;
	P = (value & 0x80)? P | NEGATIVE_FLAG : P & ~NEGATIVE_FLAG;
}

void NESCPU::executeORA(){
	A |= readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeAND(){
	A &= readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeEOR(){
	A ^= readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeASL(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op <<= 1;
	writeByteToMemory(effectiveAddress, op);
	toggleNegativeOrZeroFlag(op);
}

void NESCPU::executeASLA(){
	P = (A & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	A <<= 1;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeROL(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	P = (op & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op = (op << 1) | carry;
	writeByteToMemory(effectiveAddress, op);
	toggleNegativeOrZeroFlag(op);
}

void NESCPU::executeROLA(){
	ubyte carry = P & CARRY_FLAG;
	P = (A & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	A = (A << 1) | carry;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeROR(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	P = (op & 1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op = (op >> 1) | (carry << 7);
	writeByteToMemory(effectiveAddress, op);
	toggleNegativeOrZeroFlag(op);
}

void NESCPU::executeRORA(){
	ubyte carry = P & CARRY_FLAG;
	P = (A & 1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	A = (A >> 1) | (carry << 7);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeLSR(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op >>= 1;
	writeByteToMemory(effectiveAddress, op);
	toggleNegativeOrZeroFlag(op);
}

void NESCPU::executeLSRA(){
	P = (A & 1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	A >>= 1;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeADC(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	short tmp = A + op + carry;
	P = (tmp & 0xFF00)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	P = (((A ^ tmp) & (op ^ tmp)) & 0x80) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
	A = tmp & 0xFF;	
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeSBC(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	short tmp = A - op - !carry;
	P = (tmp & 0xFF00)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	P = (((A ^ op) & (A ^ tmp)) & 0x80) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
	A = tmp & 0xFF;	
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeBMI(){
	if(P & NEGATIVE_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBPL(){
	if((P & NEGATIVE_FLAG)==0)
		PC = effectiveAddress;
}

void NESCPU::executeBVS(){
	if(P & OVERFLOW_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBVC(){
	if((P & OVERFLOW_FLAG)==0)
		PC = effectiveAddress;
}

void NESCPU::executeBCS(){
	if(P & CARRY_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBCC(){
	if((P & CARRY_FLAG)==0)
		PC = effectiveAddress;
}

void NESCPU::executeBEQ(){
	if(P & ZERO_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBNE(){
	if((P & ZERO_FLAG)==0)
		PC = effectiveAddress;
}

void NESCPU::executeBIT(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 0x80)? P | NEGATIVE_FLAG : P & ~NEGATIVE_FLAG;
	P = (op & 0x40) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
	P = ((op & A)==0)? P | ZERO_FLAG: P & ~ZERO_FLAG;
}

void NESCPU::executeCMP(){
	byte tmp = A - readByteFromMemory(effectiveAddress);
	P = (tmp >= 0)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	toggleNegativeOrZeroFlag(tmp);
}

void NESCPU::executeCPX(){
	byte tmp = X - readByteFromMemory(effectiveAddress);
	P = (tmp >= 0)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	toggleNegativeOrZeroFlag(tmp);
}

void NESCPU::executeCPY(){
	byte tmp = Y - readByteFromMemory(effectiveAddress);
	P = (tmp >= 0)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	toggleNegativeOrZeroFlag(tmp);
}

void NESCPU::executeCLC(){
	P &= ~CARRY_FLAG;
}

void NESCPU::executeCLI(){
	P &= ~INTERRUPT_DISABLE_FLAG;
}

void NESCPU::executeCLD(){
	P &= ~DECIMAL_MODE_FLAG;
}

void NESCPU::executeCLV(){
	P &= ~OVERFLOW_FLAG;
}

void NESCPU::executeSEC(){
	P |= CARRY_FLAG;
}

void NESCPU::executeSEI(){
	P |= INTERRUPT_DISABLE_FLAG;
}

void NESCPU::executeSED(){
	P |= DECIMAL_MODE_FLAG;
}

void NESCPU::executeDEC(){
	ubyte tmp = readByteFromMemory(effectiveAddress) - 1;
	writeByteToMemory(effectiveAddress, tmp);
	toggleNegativeOrZeroFlag(tmp);
}

void NESCPU::executeDEX(){
	X--;
	toggleNegativeOrZeroFlag(X);
}

void NESCPU::executeDEY(){
	Y--;
	toggleNegativeOrZeroFlag(Y);
}

void NESCPU::executeINC(){
	ubyte tmp = readByteFromMemory(effectiveAddress) + 1;
	writeByteToMemory(effectiveAddress, tmp);
	toggleNegativeOrZeroFlag(tmp);
}

void NESCPU::executeINX(){
	X++;
	toggleNegativeOrZeroFlag(X);
}

void NESCPU::executeINY(){
	Y++;
	toggleNegativeOrZeroFlag(Y);
}

void NESCPU::executeLDA(){
	A = readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeLDX(){
	X = readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeLDY(){
	Y = readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeSTA(){
	writeByteToMemory(effectiveAddress, A);
}

void NESCPU::executeSTX(){
	writeByteToMemory(effectiveAddress, X);
}

void NESCPU::executeSTY(){
	writeByteToMemory(effectiveAddress, Y);
}

void NESCPU::executeNOP(){
}

void NESCPU::executeUNK(){
	throw "Código de operación desconocido";
}

void NESCPU::executePHA(){
	writeByteToMemory(STACK_ADDRESS+SP, A);
	SP--;
}

void NESCPU::executePHP(){
	writeByteToMemory(STACK_ADDRESS+SP, P);
	SP--;
}

void NESCPU::executePLA(){
	SP++;
	A = readByteFromMemory(STACK_ADDRESS+SP);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executePLP(){
	SP++;
	P = readByteFromMemory(STACK_ADDRESS+SP);
}

void NESCPU::executeRTS(){
	SP++;
	PC = readShortFromMemory(STACK_ADDRESS+SP);
	SP++;
	PC++;
}

void NESCPU::executeRTI(){
	SP++;
	P = readByteFromMemory(STACK_ADDRESS+SP) & ~INTERRUPT_DISABLE_FLAG;
	SP++;
	PC = readShortFromMemory(STACK_ADDRESS+SP);
	SP++;
}

void NESCPU::executeJMP(){
	PC = effectiveAddress;
}

void NESCPU::executeJSR(){
	ushort pc = PC - 1;
	writeByteToMemory(STACK_ADDRESS+SP, pc >> 8);
	SP--;
	writeByteToMemory(STACK_ADDRESS+SP, pc & 0xFF);
	SP--;
	PC = effectiveAddress;
}

void NESCPU::executeBRK(){
	writeByteToMemory(STACK_ADDRESS+SP, PC >> 8);
	SP--;
	writeByteToMemory(STACK_ADDRESS+SP, PC & 0xFF);
	SP--;
	writeByteToMemory(STACK_ADDRESS+SP, P);
	SP--;
	P &= ~INTERRUPT_DISABLE_FLAG;

	if(nmi){
		nmi = 0;
		PC =  readShortFromMemory(NMI_HANDLER_ADDRESS);
	}else
		PC = readShortFromMemory(IRQ_HANDLER_ADDRESS);
}

void NESCPU::executeTAX(){
	X = A;
	toggleNegativeOrZeroFlag(X);
}

void NESCPU::executeTAY(){
	Y = A;
	toggleNegativeOrZeroFlag(Y);
}

void NESCPU::executeTXA(){
	A = X;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeTYA(){
	Y = A;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeTSX(){
	X = SP;
	toggleNegativeOrZeroFlag(X);
}

void NESCPU::executeTXS(){
	SP = X;
}

void NESCPU::executeLAX(){
	A = X = readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeSAX(){
	writeByteToMemory(effectiveAddress, A & X);
}

void NESCPU::executeDCP(){
	ubyte op = readByteFromMemory(effectiveAddress)-1;
	writeByteToMemory(effectiveAddress, op);
	ushort tmp = A - op;
	P = (tmp >= 0)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	toggleNegativeOrZeroFlag((ubyte)tmp);
}

void NESCPU::executeISB(){
	ubyte op = readByteFromMemory(effectiveAddress)+1;
	writeByteToMemory(effectiveAddress, op);
	ubyte carry = P & CARRY_FLAG;
	ushort tmp = A - op - !carry;
	P = (tmp & 0xFF00)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	P = (((A ^ op) & (A ^ tmp)) & 0x80)? P | OVERFLOW_FLAG: P & ~OVERFLOW_FLAG;
	A = tmp && 0xFF;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeSLO(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op <<= 1;
	A |= op;
	writeByteToMemory(effectiveAddress, op);
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeRLA(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	P = (op & 0x80)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op = (op << 1) | carry;
	writeByteToMemory(effectiveAddress, op);
	A &= op;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeSRE(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 0x1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op >>= 1;
	writeByteToMemory(effectiveAddress, op);
	A ^= op;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeRRA(){
	ubyte op = readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	P = (op & 0x1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	op = (op >> 1) | (carry << 7);
	writeByteToMemory(effectiveAddress, op);
	ushort tmp = A + op + (P & CARRY_FLAG);
	P = (op & 0xFF00)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	P = (((A ^ tmp) & (op ^ tmp)) & 0x80) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
	A = op & 0xFF;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeAAC(){
	A &= readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
	P = (P  & NEGATIVE_FLAG)? P | CARRY_FLAG : P & ~CARRY_FLAG;
}

void NESCPU::executeASR(){
	A &= readByteFromMemory(effectiveAddress);
	P = (A & 0x1)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	A >>= 1;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeARR(){
	A &= readByteFromMemory(effectiveAddress);
	ubyte carry = P & CARRY_FLAG;
	A = (A >> 1) | (carry << 7);
	toggleNegativeOrZeroFlag(A);
	P = (A & 0x40)? P | CARRY_FLAG : P & ~CARRY_FLAG;
	P = ((A & 0x20) >> 5) ^ (P & CARRY_FLAG) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
}

void NESCPU::executeATX(){
	A = readByteFromMemory(effectiveAddress);
	X = A;
	toggleNegativeOrZeroFlag(A);
}

void NESCPU::executeAXS(){
	ushort tmp = (X & A) - readByteFromMemory(effectiveAddress);
	X = tmp & 0xFF;
	toggleNegativeOrZeroFlag(X);
	P = (tmp >= 0)? P | CARRY_FLAG : P & ~CARRY_FLAG;
}

void NESCPU::executeSYA(){
	ubyte op = Y & ((effectiveAddress >> 8)+1);
	if(X +  readByteFromMemory(operationAddress+1) <= 0xFF)
		writeByteToMemory(effectiveAddress, op);
}

void NESCPU::executeSXA(){
	byte op = X & ((effectiveAddress >> 8)+1);
	if(Y +  readByteFromMemory(operationAddress+1) <= 0xFF)
		writeByteToMemory(effectiveAddress, op);
}

void NESCPU::executeLAR(){
	ubyte op = readByteFromMemory(effectiveAddress) & SP;
	A = X = SP = op;
}

void NESCPU::executeAXA(){
	ubyte op = (A & X) & 7;
	writeByteToMemory(effectiveAddress, op);
}

void NESCPU::executeXAS(){
	SP = A & X;
	ubyte op = SP & (effectiveAddress >> 8);
	writeByteToMemory(effectiveAddress, op);
}

void NESCPU::executeXAA(){
	A = (A | 0xEE) & X & readByteFromMemory(effectiveAddress);
	toggleNegativeOrZeroFlag(A);
}

