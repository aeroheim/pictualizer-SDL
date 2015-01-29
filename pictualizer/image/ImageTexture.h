#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <string>

typedef std::shared_ptr<SDL_Texture> SharedTexture;

inline SharedTexture make_shared(SDL_Texture* texture) 
{
	return SharedTexture(texture, SDL_DestroyTexture);
}

class ImageTexture
{
	public:
		ImageTexture();
		~ImageTexture();

		void draw(SDL_Renderer* ren, SDL_Rect* view);
		int getWidth();
		int getHeight();
		bool setImage(SDL_Renderer* ren, std::string path);
		bool hasImage();
		void freeImage();
		void setColor(Uint8 r, Uint8 g, Uint8 b);
		void getColor(Uint8* r, Uint8* g, Uint8* b);
		void setTint(Uint8 rgb);
		void setBlur(int level);
		void getAlpha(Uint8* alpha);
		void setAlpha(Uint8 alpha);
		void setBlendMode(SDL_BlendMode blend);

		bool operator==(const ImageTexture& imageTexture);

	private:
		SharedTexture image;
		int iw;
		int ih;
};