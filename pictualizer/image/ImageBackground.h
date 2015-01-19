#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "ImageCamera.h"
#include "ImageTexture.h"
#include "../io/EventSubscriber.h"
#include "../util/utils.h"

class ImageBackground : public EventSubscriber, public EventObserver, public InputListener
{
	public:
		ImageBackground(SDL_Renderer* ren, int ww, int wh);
		~ImageBackground();
		void draw();
		void setImage(std::string path);
		void setImage(int index);
		void nextImage();
		void prevImage();
		void enqueueImage(std::string path);
		void removeImage(int index);
		void handleEvent(Event* e);

	private:
		std::vector<std::string> images;
		int imgIndex;
		SDL_Renderer* ren;
		ImageCamera imageCamera;
		ImageCamera tempCamera;
		ImageTexture image;
		ImageTexture temp;

		SDL_Keycode ACCESS_KEY = SDLK_LCTRL;
		SDL_Keycode PREV_IMG_KEY = SDLK_LEFT;
		SDL_Keycode NEXT_IMG_KEY = SDLK_RIGHT;
		Uint8 FADE_DELTA = 15;
		
		bool slideshow;
		bool fading;

		void fadeImage(ImageTexture& img, bool in, bool free = false);
};