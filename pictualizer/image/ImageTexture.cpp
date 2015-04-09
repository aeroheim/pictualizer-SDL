#include "ImageTexture.h"

ImageTexture::ImageTexture() : PControl(0, 0, 0, 0)
{
	image = nullptr;

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
	PControl::draw(ren);

	if (image)
		SDL_RenderCopy(ren, image.get(), view, nullptr);

	pollImageBuffers(ren);
}

void ImageTexture::draw(SDL_Renderer* ren)
{
	PControl::draw(ren);

	if (image)
		SDL_RenderCopy(ren, image.get(), nullptr, nullptr);

	pollImageBuffers(ren);
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

	SDL_Thread* thread = SDL_CreateThread(asyncImageLoad, nullptr, threadData);
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
		setWidth(0.0f);
		setHeight(0.0f);
	}
}

void ImageTexture::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (image)
	{
		PIntColor color = getRoundedColor();
		SDL_SetTextureColorMod(image.get(), color.r, color.g, color.b);
	}
}

void ImageTexture::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (image)
		SDL_SetTextureAlphaMod(image.get(), getRoundedAlpha());
}

void ImageTexture::setBlendMode(SDL_BlendMode blend)
{
	SDL_SetTextureBlendMode(image.get(), blend);
}

ImageTexture& ImageTexture::operator=(const ImageTexture& other)
{
	image = other.image;
	PControl::operator=(other);
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

			setBlendMode(SDL_BLENDMODE_BLEND);
			setTint(getTint());
			setAlpha(getAlpha());

			int iw, ih;
			SDL_QueryTexture(image.get(), nullptr, nullptr, &iw, &ih);
			setWidth((float) iw);
			setHeight((float) ih);

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