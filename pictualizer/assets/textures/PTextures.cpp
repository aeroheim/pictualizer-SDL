#include "PTextures.h"
#include <iostream>

namespace
{
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

	std::map<PTextureType, std::vector<std::pair<int, std::string>>> textures;
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
				textures[kv.first].push_back(std::pair < int, std::string > { w * h, path });
			}
	}

	SDL_Texture* requestTexture(PTextureType textureType, int area, SDL_Texture* currentTexture)
	{
		for (size_t i = 0; i < textures[textureType].size(); i++)
		{
			int& textureArea = textures[textureType][i].first;
			std::string& texturePath = textures[textureType][i].second;

			// Search for texture to return where its' area most closely matches the given area.
			if (area <= textureArea || i == textures[textureType].size() - 1)
			{
				// Check if current texture is also a texture of the same size.
				if (currentTexture)
				{
					int w, h;
					SDL_QueryTexture(currentTexture, NULL, NULL, &w, &h);
					int currentTextureArea = w * h;
					int prevArea = i == 0 ? 0 : textures[textureType][i - 1].first;

					// If so, no need to acquire a new texture; simply return the given texture back.
					if (currentTextureArea >= prevArea && currentTextureArea <= textureArea)
						return currentTexture;
					// Otherwise free the texture so that we can acquire a new one.
					else
						SDL_DestroyTexture(currentTexture);
				}

				return IMG_LoadTexture(renderer, texturePath.c_str());
			}
		}

		return currentTexture;
	}
}