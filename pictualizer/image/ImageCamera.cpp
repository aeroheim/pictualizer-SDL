#include "ImageCamera.h"
#include <iostream>

using namespace std;

ImageCamera::ImageCamera(int w, int h)
{
	ww = w;
	wh = h;
	view.w = ww;
	view.h = wh;
	view.x = 0;
	view.y = 0;
	panX = 0.0f;
	panY = 0.0f;
	scale = 1.0f;
	maxScale = 1.0f;
	state = ImageCameraState::MANUAL;

	registerKey(ACCESS_KEY);

	srand((unsigned int) time(NULL));
}

ImageCamera::~ImageCamera() {}

const float& ImageCamera::getScale()
{
	return scale;
}

SDL_Rect* ImageCamera::getView()
{
	return &view;
}

void ImageCamera::updateView()
{
	switch (panningState)
	{
		case CameraPanningState::LEFT:
			panX = (int) std::round(panX - panSpeed) >= 0 ? panX - panSpeed : 0;
			view.x = (int) std::round(panX);
			break;
		case CameraPanningState::RIGHT:
			panX = (int) std::round(panX + panSpeed) + view.w <= iw ? panX + panSpeed : panX;
			view.x = (int) std::round(panX);
			break;
		case CameraPanningState::TOP:
			panY = (int) std::round(panY - panSpeed) >= 0 ? panY - panSpeed : 0;
			view.y = (int) std::round(panY);
			break;
		case CameraPanningState::BOTTOM:
			panY = (int) std::round(panY + panSpeed) + view.h <= ih ? panY + panSpeed : panY;
			view.y = (int) std::round(panY);
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			panX = (int) std::round(panX + panSpeed) + view.w <= iw ? panX + panSpeed : panX;
			panY = (int) std::round(panY + panSpeed) + view.h <= ih ? panY + panSpeed : panY;
			view.x = (int) std::round(panX);
			view.y = (int) std::round(panY);
			break;
		case CameraPanningState::BOTTOM_LEFT:
			panX = (int) std::round(panX - panSpeed) >= 0 ? panX - panSpeed : 0;
			panY = (int) std::round(panY + panSpeed) + view.h <= ih ? panY + panSpeed : panY;
			view.x = (int) std::round(panX);
			view.y = (int) std::round(panY);
;			break;
		case CameraPanningState::TOP_RIGHT:
			panX = (int) std::round(panX + panSpeed) + view.w <= iw ? panX + panSpeed : panX;
			panY = (int) std::round(panY - panSpeed) >= 0 ? panY - panSpeed : 0;
			view.x = (int) std::floor(panX);
			view.y = (int) std::floor(panY);
			break;
		case CameraPanningState::TOP_LEFT:
			panX = (int) std::round(panX - panSpeed) >= 0 ? panX - panSpeed : 0;
			panY = (int) std::round(panY - panSpeed) >= 0 ? panY - panSpeed : 0;
			view.x = (int) std::floor(panX);
			view.y = (int) std::floor(panY);
			break;
	}
}

ImageCameraState ImageCamera::getState()
{
	return state;
}

void ImageCamera::setState(ImageCameraState s)
{
	state = s;

	if (s == ImageCameraState::ROAMING)
		resetPanning();
	else if (s == ImageCameraState::MANUAL)
		resetCamera();
}

CameraPanningState ImageCamera::getPanningState()
{
	return panningState;
}

void ImageCamera::resetCamera()
{
	view.x = 0;
	view.y = 0;
	calculateMaxScale();
	scale = maxScale;
	view.w = (int) std::floor(ww * scale);
	view.h = (int) std::floor(wh * scale);
}

void ImageCamera::setView(ImageTexture* image)
{
	iw = image->getWidth();
	ih = image->getHeight();
	resetCamera();

	if (state == ImageCameraState::ROAMING)
		resetPanning();
}

float ImageCamera::getPanSpeed()
{
	return panSpeed;
}

void ImageCamera::setPanSpeed(float speed)
{
	panSpeed = speed;
}

void ImageCamera::handleEvent(Event* e)
{
	if (!e->handled)
	{
		if (KeyDownEvent* keyDownEvent = dynamic_cast<KeyDownEvent*>(e))
		{
			
			setKeyHeld(keyDownEvent->key);
		}
		else if (dynamic_cast<KeyUpEvent*>(e))
		{
			KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e);
			setKeyReleased(keyUpEvent->key);
		}
		else if (dynamic_cast<MouseDownEvent*>(e))
		{
			MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e);
			if (!lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = true;
				mDownX = mouseDownEvent->x;
				mDownY = mouseDownEvent->y;
			}
		}
		else if (dynamic_cast<MouseUpEvent*> (e))
		{
			MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e);
			if (mouseUpEvent->button == SDL_BUTTON_LEFT)
				lMouseHeld = false;
		}
		else if (dynamic_cast<MouseWheelEvent*>(e))
		{
			if (keyHeld(ACCESS_KEY))
			{
				MouseWheelEvent* mouseWheelEvent = dynamic_cast<MouseWheelEvent*>(e);
				OnMouseWheel(mouseWheelEvent);
				e->handled = true;
			}
		}
		else if (dynamic_cast<MouseMotionEvent*>(e))
		{
			if (keyHeld(ACCESS_KEY) && lMouseHeld)
			{
				SDL_ShowCursor(SDL_DISABLE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e);
				OnMouseMotion(mouseMotionEvent);
				e->handled = true;
			}
			else
			{
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
		else if (dynamic_cast<WindowResizeEvent*>(e))
		{
			WindowResizeEvent* windowResizeEvent = dynamic_cast<WindowResizeEvent*>(e);
			OnWindowResize(windowResizeEvent);
			e->handled = true;
		}
	}
}

void ImageCamera::calculateMaxScale()
{
	float wRatio = (float) iw / (float) ww;
	float hRatio = (float) ih / (float) wh;

	maxScale = wRatio < hRatio ? wRatio : hRatio;
}

void ImageCamera::resetPanning()
{
	// Randomize a new panning direction.
	generateNewPanningStyle();

	float whratio = (float) ww / (float) wh;
	int maxScaleW = (int) std::floor(ww * maxScale);
	int maxScaleH = (int) std::floor(wh * maxScale);
	int minPanDist = (int) std::floor(MIN_PAN_DURATION * panSpeed * 60);

	switch (panningState)
	{
		case CameraPanningState::LEFT:		
			{
				int dist = (iw - maxScaleW) > minPanDist ? iw - maxScaleW : minPanDist;			
				int maxPanDist = dist + (int) std::floor(MAX_PAN_RANGE * panSpeed * 60);

				dist += rand() % (maxPanDist - dist);

				view.x = dist + rand() % (int) std::floor((iw - dist) * MAX_START_XY_PCT);
				view.w = iw - view.x;

				view.h = (int) std::floor((float) view.w / whratio);
				view.y = rand() % (ih - view.h);
			}
			break;
		case CameraPanningState::RIGHT:
			{
				int dist = (iw - maxScaleW) > minPanDist ? iw - maxScaleW : minPanDist;
				int maxPanDist = dist + (int) std::floor(MAX_PAN_RANGE * panSpeed * 60);

				dist += rand() % (maxPanDist - dist);

				view.x = rand() % (int) std::floor((iw - dist) * MAX_START_XY_PCT);
				view.w = (iw - dist) - view.x;

				view.h = (int) std::floor((float) view.w / whratio);
				view.y = rand() % (ih - view.h);
			}
			break;
		case CameraPanningState::TOP:
			{
				int dist = (ih - maxScaleH) > minPanDist ? ih - maxScaleH : minPanDist;
				int maxPanDist = dist + (int) std::floor(MAX_PAN_RANGE * panSpeed * 60);

				dist += rand() % (maxPanDist - dist);

				view.y = dist + rand() % (int) std::floor((ih - dist) * MAX_START_XY_PCT);
				view.h = ih - view.y;

				view.w = (int) std::floor((float) view.h * whratio);
				view.x = rand() % (iw - view.w);
			}
			break;
		case CameraPanningState::BOTTOM:
			{
				int dist = (ih - maxScaleH) > minPanDist ? ih - maxScaleH : minPanDist;
				int maxPanDist = dist + (int) std::floor(MAX_PAN_RANGE * panSpeed * 60);

				dist += rand() % (maxPanDist - dist);

				view.y = rand() % (int) std::floor((ih - dist) * MAX_START_XY_PCT);
				view.h = (ih - dist) - view.y;

				view.w = (int) std::floor((float) view.h * whratio);
				view.x = rand() % (iw - view.w);
			}
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			{

			}
			break;
		case CameraPanningState::BOTTOM_LEFT:
			break;
		case CameraPanningState::TOP_RIGHT:
			break;
		case CameraPanningState::TOP_LEFT:
			break;
	}

	panX = (float) view.x;
	panY = (float) view.y;
}

void ImageCamera::generateNewPanningStyle()
{
	panningState = (CameraPanningState) (rand() % 4);

	// panningState = CameraPanningState::RIGHT;
}

void ImageCamera::OnMouseWheel(MouseWheelEvent* e)
{
	float newScale = scale - (float) (SCROLL_SPEED * e->y);

	if (newScale > maxScale)
		scale = maxScale;
	else if (newScale < 0)
		scale = 0;
	else
	{
		scale = newScale;

		float xMaxPos = (float) (iw - ww * scale);
		float yMaxPos = (float) (ih - wh * scale);

		// Valid bounds checking on current camera position.
		if (view.x > xMaxPos)
			view.x = (int) std::floor(xMaxPos);
		else if (view.x < 0)
			view.x = 0;

		if (view.y > yMaxPos)
			view.y = (int) std::floor(yMaxPos);
		else if (view.y < 0)
			view.y = 0;

		// Zoom-in to mouse.
		float dX = ww * (1 - scale);
		float dY = wh * (1 - scale);
		float pX = (float)(e->mx) / (float)ww;
		float pY = (float)(e->my) / (float)wh;

		view.w = ww - (int)std::round(dX);
		view.h = wh - (int)std::round(dY);

		if ((float) (SCROLL_SPEED * e->y) > 0)
		{
			view.x += (int)(std::round((float)ww * SCROLL_SPEED) * pX);
			view.y += (int)(std::round((float)wh * SCROLL_SPEED) * pY);
		}
		else
		{
			view.x -= (int)(std::round((float)ww * SCROLL_SPEED) * pX);
			view.y -= (int)(std::round((float)wh * SCROLL_SPEED) * pY);
		}

		if (view.x < 0)
			view.x = 0;
		if (view.y < 0)
			view.y = 0;
	}
}

void ImageCamera::OnMouseMotion(MouseMotionEvent* e)
{
	int newPosX = view.x - e->xrel;
	int newPosY = view.y - e->yrel;

	float xMaxPos = (float) (iw - ww * scale);
	float yMaxPos = (float) (ih - wh * scale);

	if (newPosX >= 0 && newPosX <= xMaxPos)
		view.x = newPosX;
	else if (view.x < 0)
		view.x = 0;

	if (newPosY >= 0 && newPosY <= yMaxPos)
		view.y = newPosY;
	else if (view.y < 0)
		view.y = 0;
}

void ImageCamera::OnWindowResize(WindowResizeEvent* e)
{
	ww = e->ww;
	wh = e->wh;
	view.w = ww;
	view.h = wh;

	calculateMaxScale();

	if (scale > maxScale)
		scale = maxScale;

	view.w = (int) std::floor(view.w * scale);
	view.h = (int) std::floor(view.h * scale);

	float xMaxPos = (float) (iw - ww * scale);
	float yMaxPos = (float) (ih - wh * scale);

	if (view.x > xMaxPos)
		view.x = (int) std::floor(xMaxPos);
	else if (view.x < 0)
		view.x = 0;

	if (view.y > yMaxPos)
		view.y = (int) std::floor(yMaxPos);
	else if (view.y < 0)
		view.y = 0;
}
