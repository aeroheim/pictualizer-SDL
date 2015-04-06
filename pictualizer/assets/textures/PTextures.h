#pragma once

#include <map>
#include <vector>
#include <string>
#include <assert.h>
#include <SDL.h>
#include <SDL_Image.h>
#include "PTextureTypes.h"
#include "../../util/PUtils.h"

namespace PTextures
{
	void initTextures(SDL_Renderer* ren);

	bool requestTexture(PTextureType textureType, int area, SDL_Texture** currentTexture, PTextureType prevTextureType = PTextureType::NONE);
}