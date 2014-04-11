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


#if !defined(_NESCPU_H)
#define _NESCPU_H

#include "types.h"
#include "AddressingMode.h"
#include "Memory.h"
#include <fstream>

class NES;

// Cantidad de registros de entrada y salida que son respaldados en memoria
class NESCPU {
	friend class NES;
	typedef union {
		ubyte data;
		struct {
			ubyte C:1;
			ubyte Z:1;
			ubyte I:1;
			ubyte D:1;
			ubyte B:1;
			ubyte :1;
			ubyte V:1;
			ubyte N:1;
		};
	} CPUStatusReg;
public:
	// Dirección de inicio de la pila en la memoria
	static const ushort STACK_ADDRESS = 0x100;
	// Dirección de Inicio de los registros de entrada y salida en la memoria, regularmente se almacenan en este espacio los registros de la PPU
	static const ushort PPU_IO_REGISTERS_ADDRESS = 0x2000;
	// Espejo de los registros 0x2000-0x2007
	static const ushort PPU_IO_REGISTERS_MIRROR_ADDRESS = 0x2008;
	// Dirección donde se alojan los registros de la APU y Joypad
	static const ushort APU_JOY_IO_REGISTERS_ADDRESS = 0x4000;
	// Dirección final de los registros de la apu
	static const ushort APU_JOY_IO_REGISTERS_END_ADDRESS = 0x4017;
	// Dirección de inicio de la Memoria de expansión en la memoria
	static const ushort EXPANSION_ROM_ADDRESS = 0x4020;
	// Dirección de inicio del espacio utilizado para guardar partidas en la memoria
	static const ushort SRAM_ADDRESS = 0x6000;
	// Dirección de inicio del banco de memoria inferior del programa cargado en memoria
	static const ushort PRG_ROM_LB_ADDRESS = 0x8000;
	// Dirección de inicio del banco de memoria superior del programa cargado en memoria
	static const ushort PRG_ROM_UB_ADDRESS = 0xC000;
	// Tamaño de una página de memoria
	static const ushort MEM_PAGE_SIZE = 0x100;
	// Dirección de memoria donde se encuentra el manejador de interrupción para IRQ/BRK
	static const ushort IRQ_HANDLER_ADDRESS = 0xFFFE;
	// Dirección de memoria donde se encuentra el manejador de interrupción NMI
	static const ushort NMI_HANDLER_ADDRESS = 0xFFFA;
	// Dirección de memoria donde se encuentra el manejador de interrupción para un reset
	static const ushort RESET_HANDLER_ADDRESS = 0xFFFC;
	// Dirección de Inicio de la primera página de memoria
	static const ushort ZERO_PAGE_ADRESS = 0;
	// Cantidad de registros de entrada salida que son respaldados
	static const ushort IO_REGISTERS_SIZE = 8;
	// Tamaño que ocupa en memoria cada banco del programa
	static const ushort PRG_ROM_BANK_SIZE = 0x4000;
	// Tamaño del espacio de memoria usado como RAM
	static const ushort RAM_SIZE = 0x800;
	// Longitud de la memoria del cpu
	static const uint32 MEMORY_SIZE = 0x10000;
	// Marca el valor inicial del SP
	static const ubyte SP_BEGIN = 0xFF;
	// Número de ciclos que tarda cada operación
	static const ubyte CYCLES_BY_OPCODE[];
	// Constructor de la clase
	NESCPU(NES *nes);
	// Destructor de objetos
	~NESCPU();
	// Emula una intrucción que normalmente contempla varios ciclos de cpu
	void runInstruction();
	// Realiza un reinicio a nivel de software
	void softReset();
	// Realiza un reinicio a nivel de hardware
	void hardReset();
	// Retorna el contador de programa
	ushort getPC();
	// Retorna el puntero de pila
	ushort getSP();
	// retorna el registro acumulador
	ushort getA();
	// retorna el registro X
	ushort getX();
	// retorna el registro Y
	ushort getY();
	// Retorna el registro de status del procesador
	ubyte getP();
	// Procesa las interrupciones si existe alguna
	bool processInterrupt(bool _nmi);
	// tick
	void tick();
	// Activa la interrupción irq
	void activateIRQ();
	// Activa la interrupción nmi
	void activateNMI();
	// Activa la bandera para indicar una interrupción NMI
	void setNMI();
	// Activa la bandera para indicar una interrupción IRQ
	void setIRQ();
	// Desactiva la bandera para indicar una interrupción NMI
	void clearNMI();
	// Desactiva la bandera para indicar una interrupción IRQ
	void clearIRQ();
	// Escribir valores a memoria
	void writeByteToMemory(ushort address, byte value);
	//Leer un byte de la memoria
	ubyte readByteFromMemory(ushort address);
	// Lee un etero largo de la memoria
	ushort readShortFromMemory(ushort address);
private:
	// Realiza el calculo de la dirección de acuerdo al direccionamiento
	inline void calculateAddress(AddressingMode mode);
	// Instrucciones para ejecutar opcodes
	inline void executeORA();
	inline void executeAND();
	inline void executeEOR();
	inline void executeASL();
	inline void executeASLA();
	inline void executeROL();
	inline void executeROLA();
	inline void executeROR();
	inline void executeRORA();
	inline void executeLSR();
	inline void executeLSRA();
	inline void executeADC();
	inline void executeSBC();
	inline void executeBMI();
	inline void executeBPL();
	inline void executeBVS();
	inline void executeBVC();
	inline void executeBCS();
	inline void executeBCC();
	inline void executeBEQ();
	inline void executeBNE();
	inline void executeBIT();
	inline void executeCMP();
	inline void executeCPX();
	inline void executeCPY();
	inline void executeCLC();
	inline void executeCLI();
	inline void executeCLD();
	inline void executeCLV();
	inline void executeSEC();
	inline void executeSEI();
	inline void executeSED();
	inline void executeDEC();
	inline void executeDEX();
	inline void executeDEY();
	inline void executeINC();
	inline void executeINX();
	inline void executeINY();
	inline void executeLDA();
	inline void executeLDX();
	inline void executeLDY();
	inline void executeSTA();
	inline void executeSTX();
	inline void executeSTY();
	inline void executeNOP();
	inline void executeUNK();
	inline void executePHA();
	inline void executePHP();
	inline void executePLA();
	inline void executePLP();
	inline void executeRTS();
	inline void executeRTI();
	inline void executeJMP();
	inline void executeJSR();
	inline void executeBRK();
	inline void executeTAX();
	inline void executeTAY();
	inline void executeTXA();
	inline void executeTYA();
	inline void executeTSX();
	inline void executeTXS();
	inline void executeLAX();
	inline void executeSAX();
	inline void executeDCP();
	inline void executeISB();
	inline void executeSLO();
	inline void executeRLA();
	inline void executeSRE();
	inline void executeRRA();
	inline void executeAAC();
	inline void executeASR();
	inline void executeARR();
	inline void executeATX();
	inline void executeAXS();
	inline void executeSYA();
	inline void executeSXA();
	inline void executeLAR();
	inline void executeAXA();
	inline void executeXAS();
	inline void executeXAA();
	// Activa o desactiva las banderas N y Z
	void toggleNegativeOrZeroFlag(byte value);
private:
	// Es un registro de 16 bits que mantiene la dirección de la siguiente instrucción que será ejecutada. Su valor puede ser afectado por bifurcaciones, saltos, llamadas a procedimientos e interrupciones.
	ushort PC;
	// Un registro de 8 bit que mantiene la dirección del tope de la pila, realmente es un offset respecto a la dirección 0x0100 y su valor se mueve alrededor de 0x00 a 0xFF.
	ubyte SP;
	// Es un registro de 8 bit que sirve como acumulado, el cual guarda resultados de operaciones aritmetico lógicas. También puede ser usado para recuperar información de la memoria.
	ubyte A;
	// Es un registro de 8 bits usado como contador o como desplazamiento cuando se realiza direccionamiento de memoria. El Registor X puede ser usado para recuperar información de memoria o para asignar o obtener el valor del SP
	ubyte X;
	// Es un registro de 8 bits usado como contador o como desplazamiento cuando se realiza direccionamiento de memoria
	ubyte Y;
	// Contiene en sus bit valores bandera las cuales se asignan o limpian cuando una instrucción es ejecutada, los bits contienen las siguientes banderas:
	// 0-Carry Flag (C), 1-Zero Flag (Z), 2-Interrupt Disable (I), 3-Decimal Mode (D), 4-Break Command (B), 6-Overflow Flag (V), 7-Negative Flag (N)
	CPUStatusReg P;
	// RAM de la CPU
	Memory ram;
	// El equipo que agrupa todos los componentes
	NES *nes;
	// ciclos necesarios para llevar a cabo la instrucción
	ushort opcodeCycles;
	// dirección efectiva
	ushort effectiveAddress;
	// dirección de memoria de la operación actual
	ushort operationAddress;
	// indica cuando una interupción nmi debe ocurrir
	bool nmi, nmiDetected;
	// indica cuando una interrupción irq debe ocurrir
	bool irq;
	// código de operación actual
	ubyte currentOpcode;

	//std::ofstream file;
	uint32 op;
};

#endif  //_NESCPU_H
