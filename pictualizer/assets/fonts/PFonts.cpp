#include "PFonts.h"
#include <iostream>

namespace
{
	const int pointSizes[] = { 16, 32, 64, 128, 256 };

	const std::string cwd = PUtils::getcwd();

	const std::map<PFontType, const std::string> fontPaths = { { PFontType::CENTURYGOTHIC, cwd + "\\assets\\fonts\\GOTHIC.ttf" },
															{ PFontType::MPLUSLIGHT, cwd + "\\assets\\fonts\\mplus-2p-light.ttf" },
															{ PFontType::MPLUSTHIN, cwd + "\\assets\\fonts\\mplus-2p-thin.ttf" } };

	std::map<PFontType, std::vector<std::tuple<int, int, int, TTF_Font*>>> fonts;
}

namespace PFonts
{
	void initFonts()
	{
		for (auto& kv : fontPaths)
			for (int ptSize : pointSizes)
			{
				// Open the font with the specified point size.
				TTF_Font* font = TTF_OpenFont(kv.second.c_str(), ptSize);

				// Query the height of the font.
				int height = TTF_FontHeight(font);

				TTF_CloseFont(font);

				// Generate a tuple for this font with the current point size - (point size, height, ref. count, font ptr)
				fonts[kv.first].push_back(std::tuple < int, int, int, TTF_Font* > { height, ptSize, 0, nullptr });
			}
	}

	void freeFonts()
	{
		for (auto& kv : fontPaths)
			for (auto& fontTuple : fonts[kv.first])
			{
				TTF_Font* font = std::get<3>(fontTuple);

				if (font)
					TTF_CloseFont(font);
			}
	}

	TTF_Font* requestFont(PFontType fontType, int height)
	{
		for (size_t i = 0; i < fonts[fontType].size(); i++)
		{
			auto& fontTuple = fonts[fontType][i];

			int ptSizeHeight = std::get<0>(fontTuple);

			// Use the respective point size for requested font which matches the height.
			if (height <= ptSizeHeight || i == fonts[fontType].size() - 1)
			{
				int& refCount = std::get<2>(fontTuple);

				// If font has not been requested before, open it for use.
				if (refCount == 0)
				{
					int& ptSize = std::get<1>(fontTuple);
					const std::string& fontPath = fontPaths.at(fontType);

					std::get<3>(fontTuple) = TTF_OpenFont(fontPath.c_str(), ptSize);
				}

				// Update the font's respective reference count.
				++refCount;

				TTF_Font* requestedFont = std::get<3>(fontTuple);

				assert(requestedFont != nullptr);

				return requestedFont;
			}
		}

		return nullptr;
	}

	void dereferenceFont(PFontType fontType, TTF_Font* activeFont)
	{
		// Search for matching allocated font.
		for (auto& fontTuple : fonts[fontType])
		{
			TTF_Font* font = std::get<3>(fontTuple);

			if (activeFont == font)
			{
				assert(std::get<2>(fontTuple) > 0);

				// Decrement the reference count for the font.
				int& refCount = --std::get<2>(fontTuple);

				// If no references are left, close the font.
				if (refCount == 0)
				{
					TTF_CloseFont(activeFont);
					break;
				}
			}
		}
	}
}