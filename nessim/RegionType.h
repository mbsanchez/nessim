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

#ifndef REGIONTYPE_H
#define REGIONTYPE_H

#include "types.h"

class Region {
public:
	enum RegionType{
		REGION_NTSC,
		REGION_PAL,
		REGION_DENDY
	};
private:
	// Tipo de Región
	RegionType	type;
	//frames per second
	uint32 fps;
	// Reloj
	uint32 hz;
	// linea de inicio de vblank
	uint32 vBlankStartLine;
	// Ultima línea de la pantalla
	uint32 screenEndLine;
public:
	Region(RegionType region);
	RegionType getType();
	uint32 getFPS();
	uint32 getHz();
	uint32 getVBlankStartLine();
	uint32 getScreenEndLine();
};

#endif