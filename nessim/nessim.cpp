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

#include <stdio.h>
#include <tchar.h>
#include "types.h"
#include "NES.h"
#include "GamePak.h" 
#include "NESSDLDisplay.h"
#include "RegionType.h"

int main(int argc, char* argv[])
{
	NESSDLDisplay display;
	NES nes(Region::REGION_NTSC, &display);
	GamePak cartridge;

	cartridge.loadFromFile(argv[1]);
	nes.insertCartridge(&cartridge, false);
	nes.runFrame();

	return 0;
}

