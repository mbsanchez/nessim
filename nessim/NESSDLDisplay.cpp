#include "NESSDLDisplay.h"


NESSDLDisplay::NESSDLDisplay(void)
{
	prevPixel = ~0u;
	SDL_Init(SDL_INIT_VIDEO | SDL_FULLSCREEN);
    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_FULLSCREEN);
    surface = SDL_SetVideoMode(256, 240, 32,0);
}

NESSDLDisplay::~NESSDLDisplay(void)
{
}

void NESSDLDisplay::putPixel(unsigned px,unsigned py, unsigned pixel, int offset){
	if(prevPixel==~0u)
		genNESPalette();

	((uint32*) surface->pixels) [py * 256 + px] = palette[offset][prevPixel%64][pixel];
     prevPixel = pixel;
}

void NESSDLDisplay::renderFrame(unsigned py){
	if(py == 239) 
		SDL_Flip(surface);
}
