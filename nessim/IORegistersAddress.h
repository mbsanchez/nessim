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


#if !defined(_IOREGISTERSADDRESS_H)
#define _IOREGISTERSADDRESS_H


// Enumeración que mantiene las direcciones de memoria de todos los registros de entrada salida. Lee o escribe un byte desde la VRAM a la dirección actual
enum IORegistersAddress {
	PPU_CONTROL_REG1=0x2000,
	PPU_CONTROL_REG2=0x2001,
	PPU_STATUS_REG=0x2002,
	SPR_RAM_ADDRESS_REG=0x2003,
	SPR_RAM_IO_REG=0x2004,
	VRAM_ADRESS_REG1=0x2005,
	VRAM_ADRESS_REG2=0x2006,
	VRAM_IO_REG=0x2007,
	PAPU_PULSE1_CTRL_REG=0x4000,
	PAPU_PULSE1_RAMP_CTRL_REG=0x4001,
	PAPU_PULSE1_FT_REG=0x4002,
	PAPU_PULSE1_CT_REG=0x4003,
	PAPU_PULSE2_CTRL_REG=0x4004,
	PAPU_PULSE2_RAMP_CTRL_REG=0x4005,
	PAPU_PULSE2_FT_REG=0x4006,
	PAPU_PULSE2_CT_REG=0x4007,
	PAPU_TRIANGLE_CRTL_REG1=0x4008,
	PAPU_TRIANGLE_CRTL_REG2=0x4009,
	PAPU_TRIANGLE_FREQ_REG1=0x400A,
	PAPU_TRIANGLE_FREQ_REG2=0x400B,
	PAPU_NOISE_CRTL_REG1=0x400C,
	PAPU_NOISE_CRTL_REG2=0x400D,
	PAPU_NOISE_FREQ_REG1=0x400E,
	PAPU_NOISE_FREQ_REG2=0x400F,
	PAPU_DM_CTRL_REG=0x4010,
	PAPU_DM_DA_REG=0x4011,
	PAPU_DM_ADDRESS_REG=0x4012,
	PAPU_DM_DATA_LENGTH_REG=0x4013,
	PAPU_SPR_DMA_REGISTER=0x4014,
	PAPU_SND_VERT_CLK_SIG_REG=0x4015,
	JOYPAD1_PORT=0x4016,
	JOYPAD2_PORT=0x4017
};

#endif  //_IOREGISTERSADDRESS_H
