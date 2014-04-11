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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef unsigned char ubyte;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned int uint32;
typedef uint64_t uint64;
typedef signed char byte;
typedef signed int int32;
typedef int64_t int64;

enum MemoryAcessMode {
	ACESS_MODE_WRITE,
	ACESS_MODE_READ
};

#endif