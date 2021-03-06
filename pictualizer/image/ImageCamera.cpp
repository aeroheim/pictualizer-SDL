#include "ImageCamera.h"

ImageCamera::ImageCamera(int w, int h) :
	w(w),
	h(h),
	iw(0),
	ih(0),
	panX(0.0f),
	panY(0.0f),
	panSpeed(0.0f),
	scale(1.0f),
	maxScale(1.0f),
	state(ImageCameraState::MANUAL)
{
	view.w = w;
	view.h = h;
	view.x = 0;
	view.y = 0;

	registerKey(ACCESS_KEY);
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
			break;
		case CameraPanningState::TOP_RIGHT:
			panX = (int) std::round(panX + panSpeed) + view.w <= iw ? panX + panSpeed : panX;
			panY = (int) std::round(panY - panSpeed) >= 0 ? panY - panSpeed : 0;
			view.x = (int) std::round(panX);
			view.y = (int) std::round(panY);
			break;
		case CameraPanningState::TOP_LEFT:
			panX = (int) std::round(panX - panSpeed) >= 0 ? panX - panSpeed : 0;
			panY = (int) std::round(panY - panSpeed) >= 0 ? panY - panSpeed : 0;
			view.x = (int) std::round(panX);
			view.y = (int) std::round(panY);
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
	view.w = (int) std::floor(w * scale);
	view.h = (int) std::floor(w * scale);
}

void ImageCamera::setView(int iw, int ih)
{
	this->iw = iw;
	this->ih = ih;
	resetCamera();

	int imageArea = iw * ih;
	int area = w * h;

	maxPanRange = (int) std::round(imageArea / ((float) area / MAX_PAN_CONSTANT));

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
			setKeyHeld(keyDownEvent->key);
		else if (KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e))
			setKeyReleased(keyUpEvent->key);
		else if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
		{
			if (!lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = true;
				mDownX = mouseDownEvent->x;
				mDownY = mouseDownEvent->y;

				if (keyHeld(ACCESS_KEY))
					e->handled = true;
			}
		}
		else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
		{
			if (mouseUpEvent->button == SDL_BUTTON_LEFT)
				lMouseHeld = false;

			if (keyHeld(ACCESS_KEY))
				e->handled = true;
		}
		else if (MouseWheelEvent* mouseWheelEvent = dynamic_cast<MouseWheelEvent*>(e))
		{
			if (keyHeld(ACCESS_KEY))
			{	
				OnMouseWheel(mouseWheelEvent);
				e->handled = true;
			}
		}
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		{
			if (keyHeld(ACCESS_KEY) && lMouseHeld)
			{
				SDL_ShowCursor(SDL_DISABLE);
				SDL_SetRelativeMouseMode(SDL_TRUE);
				OnMouseMotion(mouseMotionEvent);
				e->handled = true;
			}
			else
			{
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
		else if (WindowResizedEvent* windowResizedEvent = dynamic_cast<WindowResizedEvent*>(e))
			OnWindowResized(windowResizedEvent);
	}
}

void ImageCamera::calculateMaxScale()
{
	float wRatio = (float) iw / (float) w;
	float hRatio = (float) ih / (float) h;

	maxScale = wRatio < hRatio ? wRatio : hRatio;
}

void ImageCamera::resetPanning()
{
	// Randomize a new panning direction.
	generateNewPanningStyle();

	float whratio = (float) w / (float) h;
	int maxScaleW = (int) std::floor(w * maxScale);
	int maxScaleH = (int) std::floor(h * maxScale);

	int minPanDist = (int) std::floor(MIN_PAN_DURATION * panSpeed * 60);
	int maxPanRangeDist = (int) std::floor(maxPanRange * panSpeed * 60);

	int wdist = (iw - maxScaleW) > minPanDist ? iw - maxScaleW : minPanDist;
	int hdist = (ih - maxScaleH) > minPanDist ? ih - maxScaleH : minPanDist;

	switch (panningState)
	{
		case CameraPanningState::LEFT:		
			{
				wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
				view.x = wdist;
				view.w = (int) std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? iw - view.x - rand() % (int) std::floor((iw - wdist) * MAX_START_XY_PCT) : iw - view.x;
				view.h = (int) std::floor((float) view.w / whratio);
				view.y = (ih - view.h) > 0 ? rand() % (ih - view.h) : 0;
			}
			break;
		case CameraPanningState::RIGHT:
			{
				wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
				view.x = (int) std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? rand() % (int) std::floor((iw - wdist) * MAX_START_XY_PCT) : 0;
				view.w = (iw - wdist) - view.x;
				view.h = (int) std::floor((float) view.w / whratio);
				view.y = (ih - view.h) > 0 ? rand() % (ih - view.h) : 0;
			}
			break;
		case CameraPanningState::TOP:
			{
				hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
				view.y = hdist;
				view.h = (int) std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? ih - view.y - rand() % (int) std::floor((ih - hdist) * MAX_START_XY_PCT) : ih - view.y;
				view.w = (int) std::floor((float) view.h * whratio);
				view.x = (iw - view.w) > 0 ? rand() % (iw - view.w) : 0;
			}
			break;
		case CameraPanningState::BOTTOM:
			{
				hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
				view.y = (int) std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? rand() % (int) std::floor((ih - hdist) * MAX_START_XY_PCT) : 0;
				view.h = (ih - hdist) - view.y;
				view.w = (int) std::floor((float) view.h * whratio);
				view.x = (iw - view.w) > 0 ? rand() % (iw - view.w) : 0;
			}
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			{
				if (wdist < hdist)
				{
					wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.x = (int) std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? rand() % (int) std::floor((iw - wdist) * MAX_START_XY_PCT) : 0;
					view.w = (iw - wdist) - view.x;
					view.h = (int) std::floor((float) view.w / whratio);
					view.y = wdist < (ih - view.h) ? rand() % (ih - view.w - wdist) : 0;
				}
				else
				{
					hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.y = (int)std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? rand() % (int)std::floor((ih - hdist) * MAX_START_XY_PCT) : 0;
					view.h = (ih - hdist) - view.y;
					view.w = (int) std::floor((float) view.h * whratio);
					view.x = hdist < (iw - view.w) ? rand() % (iw - view.w - hdist): 0;
				}
			}
			break;
		case CameraPanningState::BOTTOM_LEFT:
			{
				if (wdist < hdist)
				{
					wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.x = wdist;
					view.w = (int)std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? iw - view.x - rand() % (int)std::floor((iw - wdist) * MAX_START_XY_PCT) : iw - view.x;
					view.h = (int)std::floor((float) view.w / whratio);
					view.y = wdist < (ih - view.h) ? rand() % (ih - view.h - wdist) : 0;
				}
				else
				{
					hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.y = (int) std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? rand() % (int) std::floor((ih - hdist) * MAX_START_XY_PCT) : 0;
					view.h = (ih - hdist) - view.y;
					view.w = (int) std::floor((float) view.h * whratio);
					view.x = hdist < (iw - view.w) ? hdist + rand() % (iw - view.w - hdist) : (iw - view.w);
				}
			}
			break;
		case CameraPanningState::TOP_RIGHT:
			{
				if (wdist < hdist)
				{
					wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.x = (int) std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? rand() % (int) std::floor((iw - wdist) * MAX_START_XY_PCT) : 0;
					view.w = (iw - wdist) - view.x;
					view.h = (int) std::floor((float) view.w / whratio);
					view.y = wdist < (ih - view.h) ? wdist + rand() % (ih - view.h - wdist) : (ih - view.h);
				}
				else
				{
					hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.y = hdist;
					view.h = (int) std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? ih - view.y - rand() % (int) std::floor((ih - hdist) * MAX_START_XY_PCT) : ih - view.y;
					view.w = (int) std::floor((float) view.h * whratio);
					view.x = hdist < (iw - view.w) ? rand() % (iw - view.w - hdist) : 0;
				}
			}
			break;
		case CameraPanningState::TOP_LEFT:
			{
				if (wdist < hdist)
				{
					wdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.x = wdist;
					view.w = (int) std::floor((iw - wdist) * MAX_START_XY_PCT) > 0 ? iw - view.x - rand() % (int) std::floor((iw - wdist) * MAX_START_XY_PCT) : iw - view.x;
					view.h = (int) std::floor((float) view.w / whratio);
					view.y = wdist < (ih - view.h) ? wdist + rand() % (ih - view.h - wdist)  : (ih - view.h);
				}
				else
				{
					hdist += maxPanRangeDist > 0 ? rand() % maxPanRangeDist : 0;
					view.y = hdist;
					view.h = (int) std::floor((ih - hdist) * MAX_START_XY_PCT) > 0 ? ih - view.y - rand() % (int) std::floor((ih - hdist) * MAX_START_XY_PCT) : ih - view.y;
					view.w = (int) std::floor((float) view.h * whratio);
					view.x = hdist < (iw - view.w) ? hdist + rand() % (iw - view.w - hdist) : (iw - view.w);
				}
			}
			break;
	}

	scale = (float) view.w / w;

	panX = (float) view.x;
	panY = (float) view.y;
}

void ImageCamera::generateNewPanningStyle()
{
	if (panningState == CameraPanningState::LEFT || panningState == CameraPanningState::RIGHT)
	{
		std::vector<CameraPanningState> newStates = { CameraPanningState::TOP, CameraPanningState::BOTTOM, 
										   CameraPanningState::BOTTOM_LEFT, CameraPanningState::BOTTOM_RIGHT, 
										   CameraPanningState::TOP_LEFT, CameraPanningState::TOP_RIGHT };

		panningState = newStates[rand() % newStates.size()];
	}
	else if (panningState == CameraPanningState::BOTTOM || panningState == CameraPanningState::TOP)
	{
		std::vector<CameraPanningState> newStates = { CameraPanningState::LEFT, CameraPanningState::RIGHT,
													  CameraPanningState::BOTTOM_LEFT, CameraPanningState::BOTTOM_RIGHT,
													  CameraPanningState::TOP_LEFT, CameraPanningState::TOP_RIGHT };

		panningState = newStates[rand() % newStates.size()];
	}
	else
	{
		std::vector<CameraPanningState> newStates = { CameraPanningState::LEFT, CameraPanningState::RIGHT, CameraPanningState::BOTTOM, CameraPanningState::TOP };

		panningState = newStates[rand() % newStates.size()];
	}
}

void ImageCamera::OnMouseWheel(MouseWheelEvent* e)
{
	if (state == ImageCameraState::ROAMING)
		state = ImageCameraState::MANUAL;

	float newScale = scale - (float) (SCROLL_SPEED * e->y);

	if (newScale > maxScale)
		scale = maxScale;
	else if (newScale < 0)
		scale = 0;
	else
	{
		scale = newScale;

		float xMaxPos = (float) (iw - w * scale);
		float yMaxPos = (float) (ih - h * scale);

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
		float dX = w * (1 - scale);
		float dY = h * (1 - scale);
		float pX = (float)(e->mx) / (float) w;
		float pY = (float)(e->my) / (float) h;

		view.w = w - (int)std::round(dX);
		view.h = h - (int)std::round(dY);

		if ((float) (SCROLL_SPEED * e->y) > 0)
		{
			view.x += (int)(std::round((float) w * SCROLL_SPEED) * pX);
			view.y += (int)(std::round((float) h * SCROLL_SPEED) * pY);
		}
		else
		{
			view.x -= (int)(std::round((float) w * SCROLL_SPEED) * pX);
			view.y -= (int)(std::round((float) h * SCROLL_SPEED) * pY);
		}

		if (view.x < 0)
			view.x = 0;
		if (view.y < 0)
			view.y = 0;
	}
}

void ImageCamera::OnMouseMotion(MouseMotionEvent* e)
{
	if (state == ImageCameraState::ROAMING)
		state = ImageCameraState::MANUAL;

	int newPosX = view.x - e->xrel;
	int newPosY = view.y - e->yrel;

	float xMaxPos = (float) (iw - w * scale);
	float yMaxPos = (float) (ih - h * scale);

	if (newPosX >= 0 && newPosX <= xMaxPos)
		view.x = newPosX;
	else if (view.x < 0)
		view.x = 0;

	if (newPosY >= 0 && newPosY <= yMaxPos)
		view.y = newPosY;
	else if (view.y < 0)
		view.y = 0;
}

void ImageCamera::OnWindowResized(WindowResizedEvent* e)
{
	w = e->ww;
	h = e->wh;
	view.w = w;
	view.h = h;

	float prevMaxScale = maxScale;

	calculateMaxScale();

	// Apply max scale delta to current scale.
	scale += (maxScale - prevMaxScale);

	view.w = (int) std::floor(view.w * scale);
	view.h = (int) std::floor(view.h * scale);

	float xMaxPos = (float) (iw - w * scale);
	float yMaxPos = (float) (ih - h * scale);

	if (view.x > xMaxPos)
	{
		view.x = (int)std::floor(xMaxPos);
		panX = (float) view.x;
	}
	else if (view.x < 0)
	{
		view.x = 0;
		panX = (float) view.x;
	}

	if (view.y > yMaxPos)
	{
		view.y = (int)std::floor(yMaxPos);
		panY = (float) view.y;
	}
	else if (view.y < 0)
	{
		view.y = 0;
		panY = (float) view.y;
	}
}
