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

	// Load texture if current thread is the last image to be loaded.
	if (threadData->threadNumber == caller->loadQueueCount)
	{
		threadData->buffer->surface = IMG_Load(threadData->path.c_str());
		
		// Recheck thread number, incase another image thread was created while loading.
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

	delete threadData;

	return 0;
}

void ImageTexture::setImage(SDL_Renderer* ren, std::string path)
{
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

ImageTexture& ImageTexture::operator=(const ImageTexture& rhs)
{
	image = rhs.image;
	iw = rhs.iw;
	ih = rhs.ih;
	return *this;
}

void ImageTexture::pollImageBuffers(SDL_Renderer* ren)
{
	for (auto it = imageBuffers.begin(); it != imageBuffers.end();)
	{
		// Load from the buffer.
		if (it->first == loadQueueCount && it->second->ready)
		{
			ImageLoadReadyEvent er;
			notify(&er);

			image = make_shared(SDL_CreateTextureFromSurface(ren, it->second->surface));
			SDL_QueryTexture(image.get(), NULL, NULL, &iw, &ih);

			ImageLoadedEvent e(iw, ih);
			notify(&e);
		}

		// Cleanup buffer.
		if (it->second->ready)
		{
			SDL_FreeSurface(it->second->surface);
			delete it->second;

			if (SDL_LockMutex(mutex) == 0)
			{
				it = imageBuffers.erase(it);
				SDL_UnlockMutex(mutex);
			}
			else
				throw SDL_GetError();
		}
		else
			++it;
	}
}