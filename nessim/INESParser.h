#pragma once

#include "RomParser.h"
#include <fstream>
#include "GamePak.h"

using namespace std;

class INESParser :
	public RomParser
{
public:
	INESParser(void);
	~INESParser(void);
	void parse(ifstream &file, GamePak &cart);
};

