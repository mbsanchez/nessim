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

#if !defined(_ADDRESSINGMODE_H)
#define _ADDRESSINGMODE_H


// Modos de direccionamiento
enum AddressingMode {
	ADM_UNK = 0,	// Desconocido
	ADM_IMP,		// Implícito
	ADM_IMM,		// Inmediato
	ADM_ABS,		// Absoluto
	//ADM_AXR,		// Absoluto indexado en X solo lectura
	ADM_ABX,		// Absoluto indexado en X
	//ADM_AYR,		// Absoluto indexado en Y solo lectura
	ADM_ABY,		// Absoluto indexado en Y
	ADM_ZPG,		// Zero Page
	ADM_ZPX,		// Zero Page indexado en X
	ADM_ZPY,		// Zero Page indexado en Y
	ADM_INX,		// Indirecto indexado en X
	//ADM_IYR,		// Indirecto indexado en Y para solo lectura
	ADM_INY,		// Indirecto indexado en Y
	ADM_IND,		// Indirecto
	ADM_REL			// Relativo
};

#endif  //_ADDRESSINGMODE_H