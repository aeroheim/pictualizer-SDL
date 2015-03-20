#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "PControl.h"

class Image : public PControl
{
	public:
		Image(SDL_Texture* image, float x, float y, float w, float h);
		~Image();

		void setImage(SDL_Texture* image);
		void setImage(std::string path, SDL_Renderer* ren);

		void freeImage();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren);

	private:
		SDL_Texture* image;
		SDL_Rect dest;
};