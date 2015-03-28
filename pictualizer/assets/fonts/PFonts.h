#pragma once

#include <map>
#include <tuple>
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

	TTF_Font* requestFont(PFontType fontType, int height);
	void freeFont(PFontType fontType, TTF_Font* activeFont);
}