#pragma once

#include <float.h>
#include "ImageCameraStates.h"
#include "../io/EventSubscriber.h"
#include "../io/InputListener.h"

class ImageCamera : public EventSubscriber, public InputListener
{
	public:
		ImageCamera(int w, int h);
		~ImageCamera();

		const float& getScale();

		const SDL_Rect* getView();
		void updateView();

		ImageCameraState getState();
		void setState(ImageCameraState s);

		void resetCamera();
		void setView(SDL_Texture* img);
		void handleEvent(Event* e); 

	private:
		ImageCameraState state;
		SDL_Rect view;
		const float SCROLL_SPEED = 0.01f;
		const SDL_Keycode ACCESS_KEY = SDLK_LCTRL;
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