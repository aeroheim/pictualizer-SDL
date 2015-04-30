#include "Image.h"

Image::Image(ImageTexture image, float x, float y, float w, float h) : 
	PControl(x, y, w, h),
	dest({ getRoundedX(), getRoundedY(), getRoundedWidth(), getRoundedHeight() }),
	image(image),
	loadQueueCount(0)
{
	mutex = SDL_CreateMutex();
}

Image::Image(float x, float y, float w, float h) : Image(nullptr, x, y, w, h) {}

Image::~Image()
{
	SDL_UnlockMutex(mutex);
	SDL_DestroyMutex(mutex);
}

Image::Image(const Image& other) : PControl(other)
{
	image = other.image;
}

Image& Image::operator=(const Image& other)
{
	image = other.image;
	PControl::operator=(other);
	return *this;
}

void Image::setImage(ImageTexture image)
{
	this->image = image;

	setColor(getColor());
	setAlpha(getAlpha());
}

void Image::asyncSetImage(ImageRWops& imageRWops)
{
	++loadQueueCount;
	createAsyncLoadThread(getNewThreadData("", imageRWops));
}

void Image::asyncSetImage(std::string path)
{
	++loadQueueCount;
	createAsyncLoadThread(getNewThreadData(path, ImageRWops{ nullptr, nullptr }));
}

ImageTexture Image::getImage() const
{
	return image;
}

void Image::freeImage()
{
	if (image)
		image.reset();
}

void Image::setX(float x)
{
	PControl::setX(x);
	dest.x = getRoundedX();
}

void Image::setY(float y)
{
	PControl::setY(y);
	dest.y = getRoundedY();
}

void Image::setWidth(float w)
{
	PControl::setWidth(w);
	dest.w = getRoundedWidth();
}

void Image::setHeight(float h)
{
	PControl::setHeight(h);
	dest.h = getRoundedHeight();
}

void Image::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (image)
	{
		PIntColor color = getRoundedColor();
		SDL_SetTextureColorMod(image.get(), color.r, color.g, color.b);
	}
}

void Image::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (image)
		SDL_SetTextureAlphaMod(image.get(), getRoundedAlpha());
}

void Image::draw(SDL_Renderer* ren)
{
	draw(ren, nullptr);
}

void Image::draw(SDL_Renderer* ren, SDL_Rect* view)
{
	if (image)
		SDL_RenderCopy(ren, image.get(), view, &dest);

	pollImageBuffers(ren);
	PControl::draw(ren);
}

ImageTexture Image::makeImageTexture(SDL_Texture* texture)
{
	return ImageTexture(texture, SDL_DestroyTexture);
}


ImageTextureThreadData* Image::getNewThreadData(std::string path, ImageRWops imageRWops)
{
	ThreadSurfaceBuffer* threadImageBuffer = new ThreadSurfaceBuffer();
	threadImageBuffer->ready = false;

	ImageTextureThreadData* threadData = new ImageTextureThreadData();
	threadData->caller = this;
	threadData->path = path;
	threadData->imageRWops = imageRWops;
	threadData->threadNumber = loadQueueCount;
	threadData->buffer = threadImageBuffer;

	return threadData;
}

void Image::createAsyncLoadThread(ImageTextureThreadData* threadData)
{
	if (SDL_LockMutex(mutex) == 0)
	{
		// Store a ThreadSurfaceBuffer in imageBuffers for the new thread to be made.
		imageBuffers[threadData->threadNumber] = threadData->buffer;
		SDL_UnlockMutex(mutex);
	}
	else
		throw SDL_GetError();

	SDL_Thread* thread = SDL_CreateThread(asyncLoadImage, nullptr, threadData);
}

int Image::asyncLoadImage(void* data)
{
	ImageTextureThreadData* threadData = static_cast<ImageTextureThreadData*>(data);
	Image* caller = threadData->caller;

	// Load texture if current thread is the last image to be loaded.
	if (threadData->threadNumber == caller->loadQueueCount)
	{
		// Load surface from an RWops.
		if (threadData->imageRWops.rwops)
		{
			assert(threadData->path.empty());

			threadData->buffer->surface = IMG_Load_RW(threadData->imageRWops.rwops, 1);

			// Delete the buffer if given.
			if (threadData->imageRWops.buffer)
				delete[] threadData->imageRWops.buffer;
		}
		// Load surface from a path.
		else
		{
			assert(!threadData->imageRWops.buffer);

			threadData->buffer->surface = IMG_Load(threadData->path.c_str());
		}

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

void Image::pollImageBuffers(SDL_Renderer* ren)
{
	for (auto it = imageBuffers.begin(); it != imageBuffers.end();)
	{
		int threadNumber = it->first;
		ThreadSurfaceBuffer* buffer = it->second;

		// Load the latest surface from its respective buffer if it has finished loading.
		if (threadNumber == loadQueueCount && buffer->ready)
		{
			ImageLoadReadyEvent er;
			notify(&er);

			// Convert the surface to a texture and set the internal image to it.
			SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, buffer->surface);
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

			setImage(makeImageTexture(texture));

			int iw, ih;
			SDL_QueryTexture(image.get(), nullptr, nullptr, &iw, &ih);

			ImageLoadedEvent e(iw, ih);
			notify(&e);
		}

		// Destroy all buffers that have finished loading and aren't needed anymore.
		if (buffer->ready)
		{
			SDL_FreeSurface(buffer->surface);
			delete buffer;

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
