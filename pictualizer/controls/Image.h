#pragma once

#include <SDL.h>
#include <unordered_map>
#include <memory>
#include "PControl.h"
#include "../io/EventObserver.h"

class Image;

typedef std::shared_ptr<SDL_Texture> ImageTexture;

/*
 *	ThreadSurfaceBuffers are polled by the Image object to determine if asynchronously loaded images have finished loading.
 */
struct ThreadSurfaceBuffer
{
	SDL_Surface* surface;
	bool ready;
};

/*
 *	RWops sent to an Image contain dynamically allocated buffers that must be deleted after an RWops has been freed.
 */
struct ImageRWops
{
	SDL_RWops* rwops;
	uint8_t* buffer;
};

/*
 *  ImageTextureThreadData holds necessary data for Image threads to perform asynchronous loading.
 */
struct ImageTextureThreadData
{
	ThreadSurfaceBuffer* buffer;
	int threadNumber;
	Image* caller;

	std::string path;
	ImageRWops imageRWops;
};

class Image : public PControl, public EventObserver
{
	public:
		Image(ImageTexture image, float x, float y, float w, float h);
		Image(float x, float y, float w, float h);
		~Image();

		Image(const Image& other);
		Image& operator=(const Image& other);

		void setImage(ImageTexture image);
		void asyncSetImage(ImageRWops& imageRWops);
		void asyncSetImage(std::string path);
		ImageTexture getImage() const;

		void freeImage();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		using PControl::setColor;
		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren);
		void draw(SDL_Renderer* ren, SDL_Rect* view);

		/*
		 *	Images use shared_ptrs of SDL_Textures internally to allow for textures to be freed automatically.
		 *	This allows users to pass SDL_Textures to Image objects without having to free them manually.
		 */
		static ImageTexture makeImageTexture(SDL_Texture* texture);

		/*
		 *	Images must have their internal image buffers polled to properly load asynchronously loaded images. If an Image object's
		 *	draw() method is not called regularly, then this method MUST be called each time before loading in a new image.
		 */
		void pollImageBuffers(SDL_Renderer* ren);

	private:
		ImageTexture image;
		SDL_Rect dest;

		SDL_mutex* mutex;
		std::unordered_map<int, ThreadSurfaceBuffer*> imageBuffers;
		int loadQueueCount;

		ImageTextureThreadData* getNewThreadData(std::string path, ImageRWops imageRWops);
		void createAsyncLoadThread(ImageTextureThreadData* threadData);
		static int asyncLoadImage(void* data);
};