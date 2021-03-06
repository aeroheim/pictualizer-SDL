#pragma once

#include <SDL_ttf.h>
#include "PFontTypes.h"

namespace PFonts
{
	void initFonts();
	void freeFonts();

	bool requestFont(PFontType fontType, TTF_Font** fontptr, int height, PFontType prevFontType = PFontType::NONE);
	void incRefCount(PFontType fontType, TTF_Font* activeFont);
	void freeFont(TTF_Font** activeFont, PFontType fontType = PFontType::NONE);
}