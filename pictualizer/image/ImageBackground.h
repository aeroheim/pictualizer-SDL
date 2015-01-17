#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "ImageCamera.h"
#include "../io/EventSubscriber.h"
#include "../util/utils.h"

class ImageBackground : public EventSubscriber, public EventObserver
{
	public:
		ImageBackground(SDL_Renderer* ren, int ww, int wh);
		~ImageBackground();
		void draw();
		void setImage(std::string path);
		void enqueueImage(std::string path);
		void removeImage(int index);
		void handleEvent(Event* e);

	private:
		std::vector<std::wstring> images;
		int imgIndex;
		ImageCamera camera;
		SDL_Renderer* ren;
		SDL_Texture* bg;
		
		bool slideshow;
};