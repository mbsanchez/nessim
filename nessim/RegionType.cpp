#include "RegionType.h"

Region::Region(RegionType region){
	type = region;
	switch(region){
	case REGION_NTSC:
		fps = 60;
		hz = 236250000 / 11;
		vBlankStartLine = 241;
		screenEndLine = 261;
		break;
	case REGION_PAL:
		fps = 50;
		hz = 26601712;
		vBlankStartLine = 241;
		screenEndLine = 311;
		break;
	case REGION_DENDY:
		fps = 50;
		hz = 26601712;
		vBlankStartLine = 291;
		screenEndLine = 311;
		break;
	}
}

Region::RegionType Region::getType(){
	return type;
}

uint32 Region::getFPS(){
	return fps;
}

uint32 Region::getHz(){
	return hz;
}

uint32 Region::getVBlankStartLine(){
	return vBlankStartLine;
}

uint32 Region::getScreenEndLine(){
	return screenEndLine;
}
