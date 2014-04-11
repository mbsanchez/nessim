#pragma once
#include "types.h"

#ifndef ABSTRACTDISPLAY_H
#define ABSTRACTDISPLAY_H

class NESPPU;

class AbstractDisplay
{
	friend NESPPU;
	int joyCurrent[2], joyNext[2], joyPos[2];
protected:
	static unsigned palette[3][64][512];
	static bool palCreated;
	static void genNESPalette();
public:
	AbstractDisplay(void);
	virtual ~AbstractDisplay(void);
	virtual void putPixel(unsigned px,unsigned py, unsigned pixel, int offset)=0;
	virtual void renderFrame(unsigned py)=0;
	void joyStrobe(unsigned v);
	ubyte joyRead(unsigned idx);
};

#endif
