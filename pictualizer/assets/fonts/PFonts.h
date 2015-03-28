#pragma once

#include <map>
#include <tuple>
#include <vector>
#include <string>
#include <SDL_ttf.h>
#include "PFontTypes.h"
#include "../../util/PUtils.h"

namespace PFonts
{
	extern std::map<PFontTypes, std::vector<std::tuple<int, int, TTF_Font*>>> fonts;
	extern const std::map<PFontTypes, std::string> fontPaths;
	extern const int pointSizes[];

	extern const std::string SOURCEHANSSERIF_PATH;
	extern const std::string CENTURYGOTHIC_PATH;
	extern const std::string MPLUSLIGHT_PATH;
	extern const std::string MPLUSTHIN_PATH;

	void initPFonts();
	void freePFonts();

	std::string getFontPath(PFontTypes font);
	TTF_Font* requestFont(PFontTypes font);
	void dereferenceFont(PFontTypes font);
}