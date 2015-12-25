#pragma once

#include <SDL.h>
#include "PTextureTypes.h"

namespace PTextures
{
	void initTextures(SDL_Renderer* ren);

	bool requestTexture(PTextureType textureType, int area, SDL_Texture** currentTexture, PTextureType prevTextureType = PTextureType::NONE);
}