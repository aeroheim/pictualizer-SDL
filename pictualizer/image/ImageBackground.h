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
		void handleEvent(Event* e) override;

	private:
		SDL_Renderer* ren;
		SDL_Texture* backgroundBuffer;
		ImageCamera currentImageCamera;
		ImageCamera fadingCamera;
		Image currentImage;
		Image fadingImage;

		std::vector<std::string> images;
		int slideshowIndex = -1;
		int iw = 0;
		int ih = 0;
		
		ImageBackgroundState slideshowState = ImageBackgroundState::SLIDESHOW;
		int slideshowTimer = 30;
		int frameCount = 0;

		ImageFadeStyle fadeStyle = ImageFadeStyle::ALPHA;
		float fadingImageAlpha = 0;
		int fadeDistance;

		void drawRoamingImage();
		void drawManualImage();

		void calculateFadeDistance(float panSpeed);
		bool viewInFadeZone(ImageCamera& camera, Image& img) const;

		void checkSlideshowTimer();

		void OnWindowResized(WindowResizedEvent* e);
		void OnImageLoaded(ImageLoadedEvent* e);
		void OnImageReady();
};