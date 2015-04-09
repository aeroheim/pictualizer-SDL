#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "ImageCamera.h"
#include "ImageTexture.h"
#include "ImageBackgroundStates.h"
#include "../io/EventSubscriber.h"
#include "../util/PUtils.h"

class ImageBackground : public EventSubscriber, public EventObserver, public InputListener
{
	public:
		ImageBackground(SDL_Renderer* ren, int ww, int wh);
		~ImageBackground();
		void draw();
		void setState(ImageBackgroundState s);
		void setImage(std::string path);
		void setImage(int index);
		void nextImage();
		void prevImage();
		void enqueueImage(std::string path);
		void removeImage(int index);
		void handleEvent(Event* e);

	private:
		std::vector<std::string> images;
		int imageIndex;
		SDL_Renderer* ren;
		ImageCamera imageCamera;
		ImageCamera tempCamera;
		ImageTexture image;
		ImageTexture tempImage;

		SDL_Keycode ACCESS_KEY = SDLK_LCTRL;
		SDL_Keycode PREV_IMG_KEY = SDLK_LEFT;
		SDL_Keycode NEXT_IMG_KEY = SDLK_RIGHT;
		
		ImageBackgroundState state;
		int slideshowTimer;
		int frameCount;

		const float PAN_SPEED = 0.33f;

		const float MAX_ALPHA = 150;
		const float MIN_ALPHA = 0;
		const float FADE_DELTA = 12;
		ImageFadeStyle fadeStyle;
		float tempAlpha;
		int fadeDist;

		void calculateFadeDist(float panSpeed);
		bool viewInFadeZone(ImageCamera& camera, ImageTexture& img);

		void checkSlideshowTimer();

		void OnImageReady();
		void OnImageLoaded();
};