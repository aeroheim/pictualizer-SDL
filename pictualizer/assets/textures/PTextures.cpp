#include "PTextures.h"
#include <iostream>

namespace
{
	struct PTextureRef
	{
		std::string path;
		int size;
		int area;
	};

	const int textureSizes[] = { 32, 64, 128 };

	const std::string cwd = PUtils::getcwd();

	const std::string textureFormat(".png");

	const std::map<PTextureType, const std::string> texturePaths = { { PTextureType::AP_LEFT, cwd + "\\assets\\textures\\ap-left-" },
																	{ PTextureType::AP_RIGHT, cwd + "\\assets\\textures\\ap-right-" },
																	{ PTextureType::AP_PAUSE, cwd + "\\assets\\textures\\ap-pause-" },
																	{ PTextureType::AP_STOP, cwd + "\\assets\\textures\\ap-stop-" },
																	{ PTextureType::AP_PLAY, cwd + "\\assets\\textures\\ap-play-" },
																	{ PTextureType::AP_INFO, cwd + "\\assets\\textures\\info-" },
																	{ PTextureType::AP_PLAYLIST, cwd + "\\assets\\textures\\playlist-" },
																	{ PTextureType::AP_REPEAT, cwd + "\\assets\\textures\\repeat-" },
																	{ PTextureType::AP_SHUFFLE, cwd + "\\assets\\textures\\shuffle-" },
																	{ PTextureType::AP_VOL_UP, cwd + "\\assets\\textures\\menu-up-" },
																	{ PTextureType::AP_VOL_DOWN, cwd + "\\assets\\textures\\menu-down-" }, };

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

				std::cout << "texture request: " << textureRef.size << std::endl;

				*activeTexture = IMG_LoadTexture(renderer, textureRef.path.c_str());
				return true;
			}
		}

		// Never reached.
		assert(false);
		return false;
	}
}