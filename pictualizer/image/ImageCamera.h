#pragma once

#include "ImageCameraStates.h"
#include "ImageTexture.h"
#include "../io/EventSubscriber.h"
#include "../io/InputListener.h"

class ImageCamera : public EventSubscriber, public InputListener
{
	public:
		ImageCamera(int w, int h);
		~ImageCamera();

		const float& getScale();

		SDL_Rect* getView();
		void updateView();

		ImageCameraState getState();
		void setState(ImageCameraState s);

		void resetCamera();
		void setView(ImageTexture* image);
		void handleEvent(Event* e); 

	private:
		ImageCameraState state;
		SDL_Rect view;
		float SCROLL_SPEED = 0.01f;
		SDL_Keycode ACCESS_KEY = SDLK_LCTRL;
		float maxScale;
		float scale;
		int ww;
		int wh;
		int iw;
		int ih;

		void calculateMaxScale();
		void OnMouseWheel(MouseWheelEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
		void OnWindowResize(WindowResizeEvent* e);
};