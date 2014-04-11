#define _USE_MATH_DEFINES
#include "AbstractDisplay.h"
#include <cmath>
#include <cstring>

unsigned AbstractDisplay::palette[3][64][512]={};
bool AbstractDisplay::palCreated = false;

AbstractDisplay::AbstractDisplay(void)
{
	memset(joyCurrent, 0, 2*sizeof(int));
	memset(joyNext, 0, 2*sizeof(int));
	memset(joyPos, 0, 2*sizeof(int));
	genNESPalette();
	joyNext[0] = 0;
}

AbstractDisplay::~AbstractDisplay(void)
{
}

void AbstractDisplay::genNESPalette(){
	// The input value is a NES color index (with de-emphasis bits).
    // We need RGB values. To produce a RGB value, we emulate the NTSC circuitry.
    // For most part, this process is described at:
    //    http://wiki.nesdev.com/w/index.php/NTSC_video
    // Incidentally, this code is shorter than a table of 64*8 RGB values.
    // Caching the generated colors
	// Thaks to Joel Yliluoma - http://iki.fi/bisqwit/
	if(!palCreated)
		for(int o=0; o<3; ++o)
			for(int u=0; u<3; ++u)
				for(int p0=0; p0<512; ++p0)
					for(int p1=0; p1<64; ++p1)
					{
						// Calculate the luma and chroma by emulating the relevant circuits:
						auto s = "\372\273\32\305\35\311I\330D\357}\13D!}N";
						int y=0, i=0, q=0;
						for(int p=0; p<12; ++p) // 12 samples of NTSC signal constitute a color.
						{
							// Sample either the previous or the current pixel.
							int r = (p+o*4)%12, pixel = r < 8-u*2 ? p0 : p1; // Use pixel=p0 to disable artifacts.
							// Decode the color index.
							int c = pixel%16, l = c<0xE ? pixel/4 & 12 : 4, e=p0/64;
							// NES NTSC modulator (square wave between up to four voltage levels):
							int b = 40 + s[(c > 12*((c+8+p)%12 < 6)) + 2*!(0451326 >> p/2*3 & e) + l];
							// Ideal TV NTSC demodulator:
							y += b;
							i += b * int(std::cos(M_PI * p / 6) * 5909);
							q += b * int(std::sin(M_PI * p / 6) * 5909);
						}
						// Convert the YIQ color into RGB
						auto gammafix = [=](float f) { return f <= 0.f ? 0.f : std::pow(f, 2.2f / 1.8f); };
						auto clamp    = [](int v) { return v>255 ? 255 : v; };
						// Store color at subpixel precision
						if(u==2) palette[o][p1][p0] += 0x10000*clamp(255 * gammafix(y/1980.f + i* 0.947f/9e6f + q* 0.624f/9e6f));
						if(u==1) palette[o][p1][p0] += 0x00100*clamp(255 * gammafix(y/1980.f + i*-0.275f/9e6f + q*-0.636f/9e6f));
						if(u==0) palette[o][p1][p0] += 0x00001*clamp(255 * gammafix(y/1980.f + i*-1.109f/9e6f + q* 1.709f/9e6f));
					}
}

void AbstractDisplay::joyStrobe(uint32 v)
{
    if(v) { joyCurrent[0] = joyNext[0]; joyPos[0]=0; }
    if(v) { joyCurrent[1] = joyNext[1]; joyPos[1]=0; }
}

ubyte AbstractDisplay::joyRead(uint32 idx)
{								  //A   B    Se   St   Up   D    L    R
    static const ubyte masks[8] = {0x20,0x10,0x40,0x80,0x04,0x08,0x02,0x01};
    return ((joyCurrent[idx] & masks[joyPos[idx]++ & 7]) ? 1 : 0);
}
