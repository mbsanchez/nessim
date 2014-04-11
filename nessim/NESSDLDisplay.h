#pragma once
#include "abstractdisplay.h"
#include "SDL.h"

class NESSDLDisplay :
	public AbstractDisplay
{
	unsigned prevPixel;
	SDL_Surface *surface;
public:
	NESSDLDisplay(void);
	virtual ~NESSDLDisplay(void);
	void putPixel(unsigned px,unsigned py, unsigned pixel, int offset);
	void renderFrame(unsigned py);

};

