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


#if !defined(_NES_H)
#define _NES_H

#include "Memory.h"
#include "NESCPU.h"
#include "RegionType.h"
#include "NESPPU.h"
#include "AbstractDisplay.h"
#include "GamePak.h"

class NES {
public:
	friend class NESCPU;
	friend class NESPPU;
	// Constructor de la clase
	NES(Region::RegionType type, AbstractDisplay *screen);
	// Destructor de objetos de la clase
	~NES();
	// Retorna el componente CPU
	NESCPU* getCPU();
	// Retorna el tipo de región de sistema
	Region getRegion();
	// Emula la ejecución de un frame de vídeo
	void runFrame();
	// Sincroniza el cpu con los demás componentes de hardware
	void syncHardware();
	// inserta un cartucho en el dispositivo
	void insertCartridge(GamePak *cart, bool removePrevius);
private:
	bool dmaAcessing;
	/*La CPU del sistema*/
	NESCPU *cpu;
	/*La PPU del sistema*/
	NESPPU * ppu;
	/*Region del Sistema*/
	Region region;
	/*El objeto que será usado para mostra el vídeo en pantalla*/
	AbstractDisplay *display;
	/* El Juego insertado en la NES*/
	GamePak *cartridge;
};

#endif  //_NES_H
