#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_thread.h>
#include <unordered_map>
#include <memory>
#include <string>
#include "../io/EventObserver.h"
#include "../controls/PControl.h"

class ImageTexture;

/*
 *	SharedTexture is a shared_ptr wrapper that allows for shared_ptrs to be used on SDL_Texture classes.
 */
typedef std::shared_ptr<SDL_Texture> SharedTexture;

inline SharedTexture make_shared(SDL_Texture* texture) 
{
	return SharedTexture(texture, SDL_DestroyTexture);
}

struct ThreadSurfaceBuffer
{
	SDL_Surface* surface;
	bool ready;
};

/*
*  ImageTextureThreadData is passed to ImageTexture threads to allow for asynchronous image loading,
*/
struct ImageTextureThreadData
{
	ImageTexture* caller;
	std::string path;
	int threadNumber;
	ThreadSurfaceBuffer* buffer;
};

class ImageTexture : public PControl, public EventObserver
{
	public:
		ImageTexture();
		~ImageTexture();

		ImageTexture& operator=(const ImageTexture& other);

		void setImage(SDL_Renderer* ren, std::string path);
		bool hasImage();
		void freeImage();

		void setColor(float r, float g, float b);
		void setAlpha(float a);
		void setBlendMode(SDL_BlendMode blend);

		void draw(SDL_Renderer* ren);
		void draw(SDL_Renderer* ren, SDL_Rect* view);

	private:
		SharedTexture image;

		std::unordered_map<int, ThreadSurfaceBuffer*> imageBuffers;
		SDL_mutex* mutex;

		int loadQueueCount;
		static int asyncImageLoad(void* data);

		void pollImageBuffers(SDL_Renderer* ren);
};