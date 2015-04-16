#include "PFonts.h"

namespace
{
	struct PFontRef
	{
		TTF_Font* ref;
		int refCount;
		int ptSize;
		int height;
	};

	const int pointSizes[] = { 16, 32, 64, 128, 256 };

	const std::map<PFontType, const std::string> fontPaths = { { PFontType::CENTURYGOTHIC, "assets/fonts/GOTHIC.ttf" },
															{ PFontType::MPLUSLIGHT, "assets/fonts/mplus-2p-light.ttf" },
															{ PFontType::MPLUSTHIN, "assets/fonts/mplus-2p-thin.ttf" } };

	std::map<PFontType, std::vector<PFontRef>> fontRefs;

	void dereferenceFont(PFontRef& fontRef)
	{
		assert(fontRef.refCount> 0);

		// Decrement the reference count for the font.
		--fontRef.refCount;

		// If no references are left, close the font.
		if (fontRef.refCount == 0)
		{
			assert(fontRef.ref);

			TTF_CloseFont(fontRef.ref);
			fontRef.ref = nullptr;
		}
	}
}

namespace PFonts
{
	void initFonts()
	{
		for (auto& kv : fontPaths)
			for (const int& ptSize : pointSizes)
			{
				// Open the font with the specified point size to query its height.
				TTF_Font* font = TTF_OpenFont(kv.second.c_str(), ptSize);
				int fontHeight = TTF_FontHeight(font);
				TTF_CloseFont(font);

				fontRefs[kv.first].push_back(PFontRef{ nullptr, 0, ptSize, fontHeight });
			}
	}

	void freeFonts()
	{
		for (auto& kv : fontPaths)
			for (PFontRef& fontRef : fontRefs[kv.first])
				if (fontRef.ref)
				{
					TTF_CloseFont(fontRef.ref);
					fontRef.ref = nullptr;
				}
	}

	bool requestFont(PFontType fontType, TTF_Font** fontptr, int height, PFontType prevFontType)
	{
		if (fontType == PFontType::NONE)
		{
			*fontptr = nullptr;
			return true;
		}

		for (size_t i = 0; i < fontRefs[fontType].size(); i++)
		{
			PFontRef& fontRef = fontRefs[fontType][i];

			// Use font size with closest matching height, or largest one possible.
			if (height <= fontRef.height || i == fontRefs[fontType].size() - 1)
			{
				if (*fontptr)
				{
					// Font picked is the same as the one passed in, so do nothing.
					if (*fontptr == fontRef.ref)
						return false;
					// Decrement reference to passed font otherwise.
					else
						freeFont(fontptr, prevFontType);
				}
	
				// If font has not been requested before, open it for use.
				if (fontRef.refCount == 0)
				{
					assert(fontRef.ref == nullptr);

					const std::string& fontPath = fontPaths.at(fontType);

					fontRef.ref = TTF_OpenFont(fontPath.c_str(), fontRef.ptSize);
				}

				// Update the font's respective reference count.
				++fontRef.refCount;

				*fontptr = fontRef.ref;
				return true;
			}
		}

		// Never reached.
		assert(false);
		return false;
	}

	void incRefCount(PFontType fontType, TTF_Font* activeFont)
	{
		for (PFontRef& fontRef : fontRefs[fontType])
			if (activeFont == fontRef.ref)
			{
				++fontRef.refCount;
				break;
			}
	}

	void freeFont(TTF_Font** activeFont, PFontType fontType)
	{
		assert(*activeFont != nullptr);

		// Search within a font type if provided for a matching font.
		if (fontType != PFontType::NONE)
		{
			for (PFontRef& fontRef : fontRefs[fontType])
				if (*activeFont == fontRef.ref)
				{
					dereferenceFont(fontRef);
					*activeFont = nullptr;
					return;
				}
		}
		// Otherwise search through every font type as well.
		else
			for (auto& font_t : fontRefs)
				for (PFontRef& fontRef : fontRefs[font_t.first])
					if (*activeFont == fontRef.ref)
					{
						dereferenceFont(fontRef);
						*activeFont = nullptr;
						return;
					}
	}
}