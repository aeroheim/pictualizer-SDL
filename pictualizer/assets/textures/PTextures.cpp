#include "PTextures.h"

namespace
{
	struct PTextureRef
	{
		std::string path;
		int size;
		int area;
	};

	const int textureSizes[] = { 32, 64, 128 };

	const std::string textureFormat(".png");

	const std::map<PTextureType, const std::string> texturePaths = { { PTextureType::AP_LEFT, "assets/textures/ap-left-" },
																	{ PTextureType::AP_RIGHT, "assets/textures/ap-right-" },
																	{ PTextureType::AP_PAUSE, "assets/textures/ap-pause-" },
																	{ PTextureType::AP_STOP, "assets/textures/ap-stop-" },
																	{ PTextureType::AP_PLAY, "assets/textures/ap-play-" },
																	{ PTextureType::AP_INFO, "assets/textures/info-" },
																	{ PTextureType::AP_PLAYLIST, "assets/textures/playlist-" },
																	{ PTextureType::AP_REPEAT, "assets/textures/repeat-" },
																	{ PTextureType::AP_SHUFFLE, "assets/textures/shuffle-" },
																	{ PTextureType::AP_VOL_UP, "assets/textures/menu-up-" },
																	{ PTextureType::AP_VOL_DOWN, "assets/textures/menu-down-" }, };

	SDL_Renderer* renderer;

	std::map<PTextureType, std::vector<PTextureRef>> textures;
}

namespace PTextures
{
	void initTextures(SDL_Renderer* ren)
	{
		renderer = ren;

		for (auto& kv : texturePaths)
			for (const int& textureSize : textureSizes)
			{
				// Get the correct file path for the texture of the current textureSize.
				std::string path = kv.second + std::to_string(textureSize) + textureFormat;

				// Get the dimensions of the texture and then free it.
				int w, h;
				SDL_Texture* texture = IMG_LoadTexture(ren, path.c_str());
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
				SDL_DestroyTexture(texture);

				// Add the area and path of the texture to this size for this texture type's list.
				textures[kv.first].push_back(PTextureRef{ path, textureSize, w * h });
			}
	}

	bool requestTexture(PTextureType textureType, int area, SDL_Texture** activeTexture, PTextureType prevTextureType)
	{
		if (textureType == PTextureType::NONE)
		{
			*activeTexture = nullptr;
			return false;
		}

		// If requesting a texture type different from the previous one, free the active texture.
		if (prevTextureType != PTextureType::NONE && prevTextureType != textureType)
		{
			assert(*activeTexture);

			SDL_DestroyTexture(*activeTexture);
			*activeTexture = nullptr;
		}

		for (size_t i = 0; i < textures[textureType].size(); i++)
		{
			PTextureRef& textureRef = textures[textureType][i];

			// Search for texture to return where its' area most closely matches the given area.
			if (area <= textureRef.area || i == textures[textureType].size() - 1)
			{
				// Check if current texture is also a texture of the same size.
				if (*activeTexture)
				{
					int w, h;
					SDL_QueryTexture(*activeTexture, NULL, NULL, &w, &h);
					int activeTextureArea = w * h;
					int prevArea = i == 0 ? 0 : textures[textureType][i - 1].area;

					// If so, no need to acquire a new texture; simply return the given texture back.
					if (activeTextureArea > prevArea && activeTextureArea <= textureRef.area)
						return false;
					// Otherwise free the texture so that we can acquire a new one.
					else
						SDL_DestroyTexture(*activeTexture);
				}

				*activeTexture = IMG_LoadTexture(renderer, textureRef.path.c_str());
				return true;
			}
		}

		// Never reached.
		assert(false);
		return false;
	}
}