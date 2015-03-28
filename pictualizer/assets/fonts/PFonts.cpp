#include "PFonts.h"

namespace PFonts
{
	const std::string SOURCEHANSSERIF_PATH = "\\assets\\SourceHansSans-Light.otf";
	const std::string CENTURYGOTHIC_PATH = "\\assets\\GOTHIC.ttf";
	const std::string MPLUSLIGHT_PATH = "\\assets\\mplus-2p-light.ttf";
	const std::string MPLUSTHIN_PATH = "\\assets\\mplus-2p-thin.ttf";

	const std::map<PFontTypes, std::string> fontPaths = { { PFontTypes::SOURCEHANSSERIF, PUtils::getcwd() + SOURCEHANSSERIF_PATH },
															{ PFontTypes::CENTURYGOTHIC, PUtils::getcwd() + CENTURYGOTHIC_PATH },
															{ PFontTypes::MPLUSLIGHT, PUtils::getcwd() + MPLUSLIGHT_PATH },
															{ PFontTypes::MPLUSTHIN, PUtils::getcwd() + MPLUSTHIN_PATH } };

	const int pointSizes[] = { 16, 32, 64, 128, 256 };

	std::map<PFontTypes, std::vector<std::tuple<int, int, TTF_Font*>>> fonts;

	void initPFonts()
	{

		/*
		for (auto& kv : fontPaths)
		{
			for (int& pointSize : pointSizes)
		}
		*/
	}
}