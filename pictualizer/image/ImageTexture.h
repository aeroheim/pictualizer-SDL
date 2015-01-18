#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class ImageTexture
{
	public:
		~ImageTexture();

		void draw(SDL_Renderer* ren, SDL_Rect* view);
		bool setImage(SDL_Renderer* ren, std::string path);
		void setColor(Uint8 r, Uint8 g, Uint8 b);
		void setTint(Uint8 rgb);
		void setBlur(int level);
		void setAlpha(Uint8 alpha);
		void setBlendMode(SDL_BlendMode blend);

	private:
		SDL_Texture* image;
		int iw;
		int ih;
};