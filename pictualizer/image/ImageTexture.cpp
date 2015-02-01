#include "ImageTexture.h"
#include <iostream>
#include <ctime>

using namespace std;

ImageTexture::ImageTexture()
{
	image = NULL;
	iw = 0;
	ih = 0;
	loadQueueCount = 0;

	mutex = SDL_CreateMutex();
}

ImageTexture::~ImageTexture()
{
	if (image)
		SDL_DestroyTexture(image.get());

	SDL_UnlockMutex(mutex);
	SDL_DestroyMutex(mutex);
}

void ImageTexture::draw(SDL_Renderer* ren, SDL_Rect* view)
{ 
	if (image)
		SDL_RenderCopy(ren, image.get(), view, NULL);

	pollImageBuffers(ren);
}

int ImageTexture::getWidth()
{
	return iw;
}

int ImageTexture::getHeight()
{
	return ih;
}

int ImageTexture::asyncImageLoad(void* data)
{
	ImageTextureThreadData* threadData = static_cast<ImageTextureThreadData*>(data);
	ImageTexture* caller = threadData->caller;

	// SharedTexture asyncImage = make_shared(IMG_LoadTexture(threadData->ren, threadData->path.c_str()));
	if (threadData->threadNumber == caller->loadQueueCount)
	{
		threadData->buffer->surface = IMG_Load(threadData->path.c_str());
		
		if (threadData->buffer->surface && threadData->threadNumber == caller->loadQueueCount)
			threadData->buffer->ready = true;
		else
		{
			if (SDL_LockMutex(caller->mutex) == 0)
			{
				caller->imageBuffers.erase(threadData->threadNumber);
				SDL_UnlockMutex(caller->mutex);
			}
			else
				throw SDL_GetError();

			if (threadData->buffer->surface)
				SDL_FreeSurface(threadData->buffer->surface);

			delete threadData->buffer;
		}
	}
	else
	{
		if (SDL_LockMutex(caller->mutex) == 0)
		{
			caller->imageBuffers.erase(threadData->threadNumber);
			SDL_UnlockMutex(caller->mutex);
		}
		else
			throw SDL_GetError();

		delete threadData->buffer;
	}

	// SharedSurface asyncSurface = make_shared(IMG_Load(threadData->path.c_str()));

	// Only load the image into the buffer if we're the latest image to be requested.
	/*
	if (threadData->threadNumber == caller->loadQueueCount && SDL_LockMutex(caller->mutex) == 0)
	{
		caller->imageBuffer = asyncSurface;
	}
	*/

	/*
	if (caller.asyncLoadFlag)
	{
		caller.image = asyncImage;
		SDL_QueryTexture(caller.image.get(), NULL, NULL, &caller.iw, &caller.ih);

		cout << "image loaded" << endl;
	}
	else
	{
		caller.iw = 0;
		caller.ih = 0;
	}
	*/

	/*
	if (image)
	{
		SDL_QueryTexture(image.get(), NULL, NULL, &iw, &ih);
		return true;
	}

	iw = 0;
	ih = 0;
	return false;
	*/

	delete threadData;

	return 0;
}

void ImageTexture::setImage(SDL_Renderer* ren, std::string path)
{
	// image = make_shared(IMG_LoadTexture(ren, path.c_str()));

	ThreadSurfaceBuffer* threadImageBuffer = new ThreadSurfaceBuffer();
	threadImageBuffer->ready = false;

	ImageTextureThreadData* threadData = new ImageTextureThreadData();
	threadData->caller = this;
	threadData->path = path;
	threadData->threadNumber = ++loadQueueCount;
	threadData->buffer = threadImageBuffer;

	if (SDL_LockMutex(mutex) == 0)
	{
		imageBuffers[loadQueueCount] = threadImageBuffer;
		SDL_UnlockMutex(mutex);
	}
	else
		throw SDL_GetError();

	SDL_Thread* thread = SDL_CreateThread(asyncImageLoad, NULL, threadData);

	SDL_WaitThread(thread, NULL);

	// image = make_shared(SDL_CreateTextureFromSurface(ren, imageBuffer.get()));
}

bool ImageTexture::hasImage()
{
	if (image)
		return true;

	return false;
}

void ImageTexture::freeImage()
{
	if (image)
	{
		image.reset();
		iw = 0;
		ih = 0;
	}
}

void ImageTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(image.get(), r, g, b);
}

void ImageTexture::getColor(Uint8* r, Uint8* g, Uint8* b)
{
	SDL_GetTextureColorMod(image.get(), r, g, b);
}

void ImageTexture::setTint(Uint8 rgb)
{
	setColor(rgb, rgb, rgb);
}

void ImageTexture::getAlpha(Uint8* alpha)
{
	SDL_GetTextureAlphaMod(image.get(), alpha);
}

void ImageTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(image.get(), alpha);
}

void ImageTexture::setBlendMode(SDL_BlendMode blend)
{
	SDL_SetTextureBlendMode(image.get(), blend);
}

bool ImageTexture::operator==(const ImageTexture& imageTexture)
{
	return imageTexture.image == image;
}

void ImageTexture::pollImageBuffers(SDL_Renderer* ren)
{
	for (auto it = imageBuffers.begin(); it != imageBuffers.end();)
	{
		// Load from the buffer.
		if (it->first == loadQueueCount && it->second->ready)
		{
			image = make_shared(SDL_CreateTextureFromSurface(ren, it->second->surface));
			// image.reset(SDL_CreateTextureFromSurface(ren, imageBuffers[loadQueueCount]->surface));
			SDL_QueryTexture(image.get(), NULL, NULL, &iw, &ih);

			cout << "iw: " << iw << ", ih: " << ih << endl;
		}

		// Cleanup buffer.
		if (it->second->ready)
		{
			SDL_FreeSurface(it->second->surface);
			delete it->second;

			if (SDL_LockMutex(mutex) == 0)
				it = imageBuffers.erase(it);
			else
				throw SDL_GetError();
		}
		else
			++it;
	}
}