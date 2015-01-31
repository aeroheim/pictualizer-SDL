#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <memory>
#include <string>


class ImageTexture;

/*
 *	SharedTexture is a shared_ptr wrapper that allows for shared_ptrs to be used on SDL_Texture classes.
 */
typedef std::shared_ptr<SDL_Texture> SharedTexture;
typedef std::shared_ptr<SDL_Surface> SharedSurface;

inline SharedTexture make_shared(SDL_Texture* texture) 
{
	return SharedTexture(texture, SDL_DestroyTexture);
}

inline SharedSurface make_shared(SDL_Surface* surface)
{
	return SharedSurface(surface, SDL_FreeSurface);
}

/*
*  ImageTextureThreadData is passed to ImageTexture threads to allow for asynchronous image loading,
*/
struct ImageTextureThreadData
{
	ImageTexture* caller;
	std::string path;
	int threadNumber;
};

class ImageTexture
{
	public:
		ImageTexture();
		~ImageTexture();

		void draw(SDL_Renderer* ren, SDL_Rect* view);
		int getWidth();
		int getHeight();
		void setImage(SDL_Renderer* ren, std::string path);
		bool hasImage();
		void freeImage();
		void setColor(Uint8 r, Uint8 g, Uint8 b);
		void getColor(Uint8* r, Uint8* g, Uint8* b);
		void setTint(Uint8 rgb);
		void getAlpha(Uint8* alpha);
		void setAlpha(Uint8 alpha);
		void setBlendMode(SDL_BlendMode blend);

		bool operator==(const ImageTexture& imageTexture);

	private:
		SharedTexture image;
		int iw;
		int ih;

		SharedSurface asyncImageBuffer;
		int loadQueueCount;
		static int asyncImageLoad(void* data);
};