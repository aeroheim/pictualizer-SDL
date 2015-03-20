#include "Button.h"

Button::Button(float x, float y, float w, float h) : PControl(x, y, w, h) {}

Button::~Button() {}

void Button::handleEvent(Event* e)
{
	if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
	{
		if (mouseInside(mouseUpEvent->x, mouseUpEvent->y))
			Button::OnMouseUp();
	}
}

void Button::OnMouseUp()
{
	ButtonPressedEvent buttonPressedEvent(this);
	notify(&buttonPressedEvent);
}