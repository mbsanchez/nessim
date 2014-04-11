#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include "types.h"

class Sprite
{
	ubyte sprIndex;
	ubyte y;
	ubyte x;
	ubyte index;
	ubyte attr;
	ushort pattern;
public:
	Sprite(void);
	~Sprite(void);
	uint64 getSprIndex(){ return sprIndex; }
	ubyte getX(){ return x; }
	ubyte getY(){ return y; }
	ubyte getIndex(){ return index; }
	ubyte getAttr(){ return attr; }
	ushort getPattern(){ return pattern; }
	void setSprIndex(ubyte sprIndex){ this->sprIndex = sprIndex; }
	void setX(ubyte x){ this->x = x; }
	void setY(ubyte y){ this->y = y; }
	void setIndex(ubyte index){ this->index = index; }
	void setAttr(ubyte attr){ this->attr = attr; }
	void setPattern(ushort pattern){ this->pattern = pattern; }
};

#endif

