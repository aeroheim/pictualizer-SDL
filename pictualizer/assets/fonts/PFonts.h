#pragma once

#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include <SDL_ttf.h>
#include "PFontTypes.h"
#include "../../util/PUtils.h"

namespace PFonts
{
	void initFonts();
	void freeFonts();

	bool requestFont(PFontType fontType, TTF_Font** fontptr, int height);
	void incRefCount(PFontType fontType, TTF_Font* activeFont);
	void freeFont(TTF_Font** activeFont, PFontType fontType = PFontType::NONE);
}