#pragma once

#include <map>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_Image.h>
#include "PTextureTypes.h"
#include "../../util/PUtils.h"

namespace PTextures
{
	void initTextures(SDL_Renderer* ren);

	SDL_Texture* requestTexture(PTextureType textureType, int area, SDL_Texture* currentTexture = nullptr);
}