#pragma once

#ifndef ROMPARSER_H
#define ROMPARSER_H

#include <fstream>
#include "GamePak.h"

using namespace std;

class RomParser{
public:
	RomParser();
	~RomParser();
	virtual void parse(ifstream &file, GamePak &cart)=0;
};

#endif

