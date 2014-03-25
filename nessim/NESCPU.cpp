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
		CASE_OPCODE(00,BRK, AddressingMode::ADM_IMP)	CASE_OPCODE(01,ORA, AddressingMode::ADM_INX)	
		CASE_OPCODE(02,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(03,SLO, AddressingMode::ADM_INX)	
		CASE_OPCODE(04,NOP, AddressingMode::ADM_ZPG)	CASE_OPCODE(05,ORA, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(06,ASL, AddressingMode::ADM_ZPG)	CASE_OPCODE(07,SLO, AddressingMode::ADM_ZPG)
		CASE_OPCODE(08,PHP, AddressingMode::ADM_IMP)	CASE_OPCODE(09,ORA, AddressingMode::ADM_IMM)	
		CASE_OPCODE(0A,ASLA, AddressingMode::ADM_IMP)	CASE_OPCODE(0B,AAC, AddressingMode::ADM_IMM)	
		CASE_OPCODE(0C,NOP, AddressingMode::ADM_ABS)	CASE_OPCODE(0D,ORA, AddressingMode::ADM_ABS)	
		CASE_OPCODE(0E,ASL, AddressingMode::ADM_ABS)	CASE_OPCODE(0F,SLO, AddressingMode::ADM_ABS)
		CASE_OPCODE(10,BPL, AddressingMode::ADM_REL)	CASE_OPCODE(11,ORA, AddressingMode::ADM_INY)	
		CASE_OPCODE(12,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(13,SLO, AddressingMode::ADM_INY)	
		CASE_OPCODE(14,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(15,ORA, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(16,ASL, AddressingMode::ADM_ZPX)	CASE_OPCODE(17,SLO, AddressingMode::ADM_ZPX)
		CASE_OPCODE(18,CLC, AddressingMode::ADM_IMP)	CASE_OPCODE(19,ORA, AddressingMode::ADM_ABY)	
		CASE_OPCODE(1A,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(1B,SLO, AddressingMode::ADM_ABY)	
		CASE_OPCODE(1C,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(1D,ORA, AddressingMode::ADM_ABX)	
		CASE_OPCODE(1E,ASL, AddressingMode::ADM_ABX)	CASE_OPCODE(1F,SLO, AddressingMode::ADM_ABX)
		CASE_OPCODE(20,JSR, AddressingMode::ADM_ABS)	CASE_OPCODE(21,AND, AddressingMode::ADM_INX)	
		CASE_OPCODE(22,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(23,RLA, AddressingMode::ADM_INX)	
		CASE_OPCODE(24,BIT, AddressingMode::ADM_ZPG)	CASE_OPCODE(25,AND, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(26,ROL, AddressingMode::ADM_ZPG)	CASE_OPCODE(27,RLA, AddressingMode::ADM_ZPG)
		CASE_OPCODE(28,PLP, AddressingMode::ADM_IMP)	CASE_OPCODE(29,AND, AddressingMode::ADM_IMM)	
		CASE_OPCODE(2A,ROLA, AddressingMode::ADM_IMP)	CASE_OPCODE(2B,AAC, AddressingMode::ADM_IMM)	
		CASE_OPCODE(2C,BIT, AddressingMode::ADM_ABS)	CASE_OPCODE(2D,AND, AddressingMode::ADM_ABS)	
		CASE_OPCODE(2E,ROL, AddressingMode::ADM_ABS)	CASE_OPCODE(2F,RLA, AddressingMode::ADM_ABS)
		CASE_OPCODE(30,BMI, AddressingMode::ADM_REL)	CASE_OPCODE(31,AND, AddressingMode::ADM_INY)	
		CASE_OPCODE(32,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(33,RLA, AddressingMode::ADM_INY)	
		CASE_OPCODE(34,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(35,AND, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(36,ROL, AddressingMode::ADM_ZPX)	CASE_OPCODE(37,RLA, AddressingMode::ADM_ZPX)
		CASE_OPCODE(38,SEC, AddressingMode::ADM_IMP)	CASE_OPCODE(39,AND, AddressingMode::ADM_ABY)	
		CASE_OPCODE(3A,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(3B,RLA, AddressingMode::ADM_ABY)	
		CASE_OPCODE(3C,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(3D,AND, AddressingMode::ADM_ABX)	
		CASE_OPCODE(3E,ROL, AddressingMode::ADM_ABX)	CASE_OPCODE(3F,RLA, AddressingMode::ADM_ABX)
		CASE_OPCODE(40,RTI, AddressingMode::ADM_IMP)	CASE_OPCODE(41,EOR, AddressingMode::ADM_INX)	
		CASE_OPCODE(42,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(43,SRE, AddressingMode::ADM_INX)	
		CASE_OPCODE(44,NOP, AddressingMode::ADM_ZPG)	CASE_OPCODE(45,EOR, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(46,LSR, AddressingMode::ADM_ZPG)	CASE_OPCODE(47,SRE, AddressingMode::ADM_ZPG)
		CASE_OPCODE(48,PHA, AddressingMode::ADM_IMP)	CASE_OPCODE(49,EOR, AddressingMode::ADM_IMM)	
		CASE_OPCODE(4A,LSRA, AddressingMode::ADM_IMP)	CASE_OPCODE(4B,ASR, AddressingMode::ADM_IMM)	
		CASE_OPCODE(4C,JMP, AddressingMode::ADM_ABS)	CASE_OPCODE(4D,EOR, AddressingMode::ADM_ABS)	
		CASE_OPCODE(4E,LSR, AddressingMode::ADM_ABS)	CASE_OPCODE(4F,SRE, AddressingMode::ADM_ABS)
		CASE_OPCODE(50,BVC, AddressingMode::ADM_REL)	CASE_OPCODE(51,EOR, AddressingMode::ADM_INY)	
		CASE_OPCODE(52,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(53,SRE, AddressingMode::ADM_INY)	
		CASE_OPCODE(54,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(55,EOR, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(56,LSR, AddressingMode::ADM_ZPX)	CASE_OPCODE(57,SRE, AddressingMode::ADM_ZPX)
		CASE_OPCODE(58,CLI, AddressingMode::ADM_IMP)	CASE_OPCODE(59,EOR, AddressingMode::ADM_ABY)	
		CASE_OPCODE(5A,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(5B,SRE, AddressingMode::ADM_ABY)	
		CASE_OPCODE(5C,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(5D,EOR, AddressingMode::ADM_ABX)	
		CASE_OPCODE(5E,LSR, AddressingMode::ADM_ABX)	CASE_OPCODE(5F,SRE, AddressingMode::ADM_ABX)
		CASE_OPCODE(60,RTS, AddressingMode::ADM_IMP)	CASE_OPCODE(61,ADC, AddressingMode::ADM_INX)	
		CASE_OPCODE(62,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(63,RRA, AddressingMode::ADM_INX)	
		CASE_OPCODE(64,NOP, AddressingMode::ADM_ZPG)	CASE_OPCODE(65,ADC, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(66,ROR, AddressingMode::ADM_ZPG)	CASE_OPCODE(67,RRA, AddressingMode::ADM_ZPG)
		CASE_OPCODE(68,PLA, AddressingMode::ADM_IMP)	CASE_OPCODE(69,ADC, AddressingMode::ADM_IMM)	
		CASE_OPCODE(6A,RORA, AddressingMode::ADM_IMP)	CASE_OPCODE(6B,ARR, AddressingMode::ADM_IMM)	
		CASE_OPCODE(6C,JMP, AddressingMode::ADM_IND)	CASE_OPCODE(6D,ADC, AddressingMode::ADM_ABS)	
		CASE_OPCODE(6E,ROR, AddressingMode::ADM_ABS)	CASE_OPCODE(6F,RRA, AddressingMode::ADM_ABS)
		CASE_OPCODE(70,BVS, AddressingMode::ADM_REL)	CASE_OPCODE(71,ADC, AddressingMode::ADM_INY)	
		CASE_OPCODE(72,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(73,RRA, AddressingMode::ADM_INY)	
		CASE_OPCODE(74,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(75,ADC, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(76,ROR, AddressingMode::ADM_ZPX)	CASE_OPCODE(77,RRA, AddressingMode::ADM_ZPX)
		CASE_OPCODE(78,SEI, AddressingMode::ADM_IMP)	CASE_OPCODE(79,ADC, AddressingMode::ADM_ABY)	
		CASE_OPCODE(7A,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(7B,RRA, AddressingMode::ADM_ABY)	
		CASE_OPCODE(7C,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(7D,ADC, AddressingMode::ADM_ABX)	
		CASE_OPCODE(7E,ROR, AddressingMode::ADM_ABX)	CASE_OPCODE(7F,RRA, AddressingMode::ADM_ABX)
		CASE_OPCODE(80,NOP, AddressingMode::ADM_IMM)	CASE_OPCODE(81,STA, AddressingMode::ADM_INX)	
		CASE_OPCODE(82,NOP, AddressingMode::ADM_IMM)	CASE_OPCODE(83,SAX, AddressingMode::ADM_INX)	
		CASE_OPCODE(84,STY, AddressingMode::ADM_ZPG)	CASE_OPCODE(85,STA, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(86,STX, AddressingMode::ADM_ZPG)	CASE_OPCODE(87,SAX, AddressingMode::ADM_ZPG)
		CASE_OPCODE(88,DEY, AddressingMode::ADM_IMP)	CASE_OPCODE(89,NOP, AddressingMode::ADM_IMM)	
		CASE_OPCODE(8A,TXA, AddressingMode::ADM_IMP)	CASE_OPCODE(8B,XAA, AddressingMode::ADM_IMM)	
		CASE_OPCODE(8C,STY, AddressingMode::ADM_ABS)	CASE_OPCODE(8D,STA, AddressingMode::ADM_ABS)	
		CASE_OPCODE(8E,STX, AddressingMode::ADM_ABS)	CASE_OPCODE(8F,SAX, AddressingMode::ADM_ABS)
		CASE_OPCODE(90,BCC, AddressingMode::ADM_REL)	CASE_OPCODE(91,STA, AddressingMode::ADM_INY)	
		CASE_OPCODE(92,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(93,AXA, AddressingMode::ADM_INY)	
		CASE_OPCODE(94,STY, AddressingMode::ADM_ZPX)	CASE_OPCODE(95,STA, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(96,STX, AddressingMode::ADM_ZPY)	CASE_OPCODE(97,SAX, AddressingMode::ADM_ZPY)
		CASE_OPCODE(98,TYA, AddressingMode::ADM_IMP)	CASE_OPCODE(99,STA, AddressingMode::ADM_ABY)	
		CASE_OPCODE(9A,TXS, AddressingMode::ADM_IMP)	CASE_OPCODE(9B,XAS, AddressingMode::ADM_ABY)	
		CASE_OPCODE(9C,SYA, AddressingMode::ADM_ABX)	CASE_OPCODE(9D,STA, AddressingMode::ADM_ABX)	
		CASE_OPCODE(9E,SXA, AddressingMode::ADM_ABY)	CASE_OPCODE(9F,AXA, AddressingMode::ADM_ABY)
		CASE_OPCODE(A0,LDY, AddressingMode::ADM_IMM)	CASE_OPCODE(A1,LDA, AddressingMode::ADM_INX)	
		CASE_OPCODE(A2,LDX, AddressingMode::ADM_IMM)	CASE_OPCODE(A3,LAX, AddressingMode::ADM_INX)	
		CASE_OPCODE(A4,LDY, AddressingMode::ADM_ZPG)	CASE_OPCODE(A5,LDA, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(A6,LDX, AddressingMode::ADM_ZPG)	CASE_OPCODE(A7,LAX, AddressingMode::ADM_ZPG)
		CASE_OPCODE(A8,TAY, AddressingMode::ADM_IMP)	CASE_OPCODE(A9,LDA, AddressingMode::ADM_IMM)	
		CASE_OPCODE(AA,TAX, AddressingMode::ADM_IMP)	CASE_OPCODE(AB,ATX, AddressingMode::ADM_IMM)	
		CASE_OPCODE(AC,LDY, AddressingMode::ADM_ABS)	CASE_OPCODE(AD,LDA, AddressingMode::ADM_ABS)	
		CASE_OPCODE(AE,LDX, AddressingMode::ADM_ABS)	CASE_OPCODE(AF,LAX, AddressingMode::ADM_ABS)
		CASE_OPCODE(B0,BCS, AddressingMode::ADM_REL)	CASE_OPCODE(B1,LDA, AddressingMode::ADM_INY)	
		CASE_OPCODE(B2,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(B3,LAX, AddressingMode::ADM_INY)	
		CASE_OPCODE(B4,LDY, AddressingMode::ADM_ZPX)	CASE_OPCODE(B5,LDA, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(B6,LDX, AddressingMode::ADM_ZPY)	CASE_OPCODE(B7,LAX, AddressingMode::ADM_ZPY)
		CASE_OPCODE(B8,CLV, AddressingMode::ADM_IMP)	CASE_OPCODE(B9,LDA, AddressingMode::ADM_ABY)	
		CASE_OPCODE(BA,TSX, AddressingMode::ADM_IMP)	CASE_OPCODE(BB,LAR, AddressingMode::ADM_ABY)	
		CASE_OPCODE(BC,LDY, AddressingMode::ADM_ABX)	CASE_OPCODE(BD,LDA, AddressingMode::ADM_ABX)	
		CASE_OPCODE(BE,LDX, AddressingMode::ADM_ABY)	CASE_OPCODE(BF,LAX, AddressingMode::ADM_ABY)
		CASE_OPCODE(C0,CPY, AddressingMode::ADM_IMM)	CASE_OPCODE(C1,CMP, AddressingMode::ADM_INX)	
		CASE_OPCODE(C2,NOP, AddressingMode::ADM_IMM)	CASE_OPCODE(C3,DCP, AddressingMode::ADM_INX)	
		CASE_OPCODE(C4,CPY, AddressingMode::ADM_ZPG)	CASE_OPCODE(C5,CMP, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(C6,DEC, AddressingMode::ADM_ZPG)	CASE_OPCODE(C7,DCP, AddressingMode::ADM_ZPG)
		CASE_OPCODE(C8,INY, AddressingMode::ADM_IMP)	CASE_OPCODE(C9,CMP, AddressingMode::ADM_IMM)	
		CASE_OPCODE(CA,DEX, AddressingMode::ADM_IMP)	CASE_OPCODE(CB,AXS, AddressingMode::ADM_IMM)	
		CASE_OPCODE(CC,CPY, AddressingMode::ADM_ABS)	CASE_OPCODE(CD,CMP, AddressingMode::ADM_ABS)	
		CASE_OPCODE(CE,DEC, AddressingMode::ADM_ABS)	CASE_OPCODE(CF,DCP, AddressingMode::ADM_ABS)
		CASE_OPCODE(D0,BNE, AddressingMode::ADM_REL)	CASE_OPCODE(D1,CMP, AddressingMode::ADM_INY)	
		CASE_OPCODE(D2,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(D3,DCP, AddressingMode::ADM_INY)	
		CASE_OPCODE(D4,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(D5,CMP, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(D6,DEC, AddressingMode::ADM_ZPX)	CASE_OPCODE(D7,DCP, AddressingMode::ADM_ZPX)
		CASE_OPCODE(D8,CLD, AddressingMode::ADM_IMP)	CASE_OPCODE(D9,CMP, AddressingMode::ADM_ABY)	
		CASE_OPCODE(DA,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(DB,DCP, AddressingMode::ADM_ABY)	
		CASE_OPCODE(DC,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(DD,CMP, AddressingMode::ADM_ABX)	
		CASE_OPCODE(DE,DEC, AddressingMode::ADM_ABX)	CASE_OPCODE(DF,DCP, AddressingMode::ADM_ABX)
		CASE_OPCODE(E0,CPX, AddressingMode::ADM_IMM)	CASE_OPCODE(E1,SBC, AddressingMode::ADM_INX)	
		CASE_OPCODE(E2,NOP, AddressingMode::ADM_IMM)	CASE_OPCODE(E3,ISB, AddressingMode::ADM_INX)	
		CASE_OPCODE(E4,CPX, AddressingMode::ADM_ZPG)	CASE_OPCODE(E5,SBC, AddressingMode::ADM_ZPG)	
		CASE_OPCODE(E6,INC, AddressingMode::ADM_ZPG)	CASE_OPCODE(E7,ISB, AddressingMode::ADM_ZPG)
		CASE_OPCODE(E8,INX, AddressingMode::ADM_IMP)	CASE_OPCODE(E9,SBC, AddressingMode::ADM_IMM)	
		CASE_OPCODE(EA,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(EB,SBC, AddressingMode::ADM_IMM)	
		CASE_OPCODE(EC,CPX, AddressingMode::ADM_ABS)	CASE_OPCODE(ED,SBC, AddressingMode::ADM_ABS)	
		CASE_OPCODE(EE,INC, AddressingMode::ADM_ABS)	CASE_OPCODE(EF,ISB, AddressingMode::ADM_ABS)
		CASE_OPCODE(F0,BEQ, AddressingMode::ADM_REL)	CASE_OPCODE(F1,SBC, AddressingMode::ADM_INY)	
		CASE_OPCODE(F2,UNK, AddressingMode::ADM_UNK)	CASE_OPCODE(F3,ISB, AddressingMode::ADM_INY)	
		CASE_OPCODE(F4,NOP, AddressingMode::ADM_ZPX)	CASE_OPCODE(F5,SBC, AddressingMode::ADM_ZPX)	
		CASE_OPCODE(F6,INC, AddressingMode::ADM_ZPX)	CASE_OPCODE(F7,ISB, AddressingMode::ADM_ZPX)
		CASE_OPCODE(F8,SED, AddressingMode::ADM_IMP)	CASE_OPCODE(F9,SBC, AddressingMode::ADM_ABY)	
		CASE_OPCODE(FA,NOP, AddressingMode::ADM_IMP)	CASE_OPCODE(FB,ISB, AddressingMode::ADM_ABY)	
		CASE_OPCODE(FC,NOP, AddressingMode::ADM_ABX)	CASE_OPCODE(FD,SBC, AddressingMode::ADM_ABX)	
		CASE_OPCODE(FE,INC, AddressingMode::ADM_ABX)	CASE_OPCODE(FF,ISB, AddressingMode::ADM_ABX)
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

ushort NESCPU::getP() {
	return P;
}

uint64 NESCPU::getCycles(){
	return cycles;
}

void NESCPU::writeByteToMemory(ushort address, byte value){
	nes->getCpuMemory()->writeByte(address, value);

	if(address < RAM_SIZE){
		nes->getCpuMemory()->makeMirrorOfAddress(address, IO_REGISTERS_ADDRESS, RAM_SIZE);
	}else if(address >= IO_REGISTERS_ADDRESS && address < IO_REGISTERS_ADDRESS+IO_REGISTERS_SIZE){
		nes->getCpuMemory()->makeMirrorOfAddress(address, 0x4000, IO_REGISTERS_SIZE);
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

void NESCPU::NESCPU::executeORA(){
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
	if(P & NEGATIVE_FLAG==0)
		PC = effectiveAddress;
}

void NESCPU::executeBVS(){
	if(P & OVERFLOW_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBVC(){
	if(P & OVERFLOW_FLAG==0)
		PC = effectiveAddress;
}

void NESCPU::executeBCS(){
	if(P & CARRY_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBCC(){
	if(P & CARRY_FLAG==0)
		PC = effectiveAddress;
}

void NESCPU::executeBEQ(){
	if(P & ZERO_FLAG)
		PC = effectiveAddress;
}

void NESCPU::executeBNE(){
	if(P & ZERO_FLAG==0)
		PC = effectiveAddress;
}

void NESCPU::executeBIT(){
	ubyte op = readByteFromMemory(effectiveAddress);
	P = (op & 0x80)? P | NEGATIVE_FLAG : P & ~NEGATIVE_FLAG;
	P = (op & 0x40) ? P | OVERFLOW_FLAG : P & ~OVERFLOW_FLAG;
	P = (op & A==0)? P | ZERO_FLAG: P & ~ZERO_FLAG;
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
	toggleNegativeOrZeroFlag(tmp);
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

