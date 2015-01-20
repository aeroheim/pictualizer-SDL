#pragma once

#include <cstdlib>
#include <ctime>
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
		CameraPanningState panningState;
		SDL_Rect view;
		float SCROLL_SPEED = 0.01f;
		SDL_Keycode ACCESS_KEY = SDLK_LCTRL;
		float maxScale;
		float scale;
		int ww;
		int wh;
		int iw;
		int ih;

		int fadeZone;

		const float MAX_START_X_PCT = .25;
		const float MAX_START_Y_PCT = .25;
		const int PAN_SPEED = 1;
		const int MAX_PAN_DURATION = 15;
		const int MIN_PAN_DURATION = 10;
		float maxPanScale;
		float minPanScale;

		void calculateMaxScale();
		void calculateFadeZone();
		void calculatePanningScales(int dist);

		void generateNewPanningStyle();
		void resetPanningCamera();

		void OnMouseWheel(MouseWheelEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
		void OnWindowResize(WindowResizeEvent* e);
};