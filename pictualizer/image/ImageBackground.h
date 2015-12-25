#pragma once               

#include <SDL.h>
#include "ImageCamera.h"
#include "ImageBackgroundStates.h"
#include "../controls/Image.h"
#include "../io/EventSubscriber.h"

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
		SDL_Renderer* ren;
		SDL_Texture* background;
		ImageCamera imageCamera;
		ImageCamera tempCamera;
		Image image;
		Image tempImage;

		std::vector<std::string> images;
		int imageIndex;
		int iw;
		int ih;
		
		ImageBackgroundState state;
		int slideshowTimer;
		int frameCount;

		ImageFadeStyle fadeStyle;
		float tempAlpha;
		int fadeDist;

		void calculateFadeDist(float panSpeed);
		bool viewInFadeZone(ImageCamera& camera, Image& img);

		void checkSlideshowTimer();

		void OnWindowResized(WindowResizedEvent* e);
		void OnImageReady();
		void OnImageLoaded(ImageLoadedEvent* e);
};