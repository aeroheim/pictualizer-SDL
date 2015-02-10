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

		CameraPanningState getPanningState();

		void resetCamera();
		void setView(ImageTexture* image);

		float getPanSpeed();
		void setPanSpeed(float speed);

		void resetPanning();

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

		float MAX_START_XY_PCT = .10f;
		float panSpeed;
		int maxPanRange;
		int MAX_PAN_CONSTANT = 4;
		int MIN_PAN_DURATION = 10;
		float panX;
		float panY;

		void calculateMaxScale();
		void generateNewPanningStyle();
		
		void OnMouseWheel(MouseWheelEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
		void OnWindowResize(WindowResizeEvent* e);
};