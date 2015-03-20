#include "Button.h"

Button::Button(float x, float y, float w, float h) : PControl(x, y, w, h) {}

Button::~Button() {}

void Button::handleEvent(Event* e)
{
	if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
	{
		if (mouseInside(mouseDownEvent->x, mouseDownEvent->y) && !lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
		{
			lMouseHeld = true;
			mDownX = mouseDownEvent->x;
			mDownY = mouseDownEvent->y;

			e->handled = true;
		}
	}
	else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
	{
		if (lMouseHeld && mouseUpEvent->button == SDL_BUTTON_LEFT)
		{
			lMouseHeld = false;
			Button::OnMouseUp();

			e->handled = true;
		}
	}
	else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
	{
		if (lMouseHeld)
			e->handled = true;
	}
}

void Button::OnMouseUp()
{
	ButtonPressedEvent buttonPressedEvent(this);
	notify(&buttonPressedEvent);
}