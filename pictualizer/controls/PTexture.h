#pragma once

#include <SDL.h>
#include "PControl.h"
#include "../assets/textures/PTextures.h"

class PTexture : public PControl
{
	public:
		PTexture(PTextureType textureType, float x, float y, float w, float h);
		PTexture(float x, float y, float w, float h);
		~PTexture();

		PTexture(const PTexture& other);
		PTexture& operator=(const PTexture& other);

		void setTexture(PTextureType textureType);

		void freeTexture();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren);

	private:
		PTextureType textureType;
		SDL_Texture* texture;
		SDL_Rect dest;
};