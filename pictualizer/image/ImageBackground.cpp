#include "ImageBackground.h"

ImageBackground::ImageBackground(SDL_Renderer* ren, int ww, int wh) : ren(ren), camera(ww, wh)
{
	bg = NULL;
	imgIndex = -1;
	slideshow = false;
	addSubscriber(&camera);
}

ImageBackground::~ImageBackground() {}

void ImageBackground::draw()
{
	SDL_RenderCopy(ren, bg, camera.getView(), NULL);
}

void ImageBackground::setImage(std::string path)
{
	// Destroy previous texture
	if (bg)
		SDL_DestroyTexture(bg);

	// Load new texture
	bg = IMG_LoadTexture(ren, path.c_str());

	camera.setView(bg);
}

void ImageBackground::enqueueImage(std::string path)
{

}

void ImageBackground::removeImage(int index)
{

}

void ImageBackground::handleEvent(Event* e)
{
	notify(e);

	if (!e->handled)
	{
		if (dynamic_cast<FileDropEvent*>(e))
		{
			FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e);
			if (utils::pathIsImage(fileDropEvent->path))
			{
				setImage(fileDropEvent->path);
				e->handled = true;
			}
		}
	}
}