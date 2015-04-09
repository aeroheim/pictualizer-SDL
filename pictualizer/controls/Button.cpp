#include "Button.h"

Button::Button(float x, float y, float w, float h) : PControl(x, y, w, h), style(ButtonStyle::CLICK), toggled(false) {}

Button::~Button() {}

bool Button::isToggled()
{
	return toggled;
}

void Button::setButtonStyle(ButtonStyle s)
{
	style = s;

	if (style == ButtonStyle::CLICK)
		toggled = false;
}

ButtonStyle Button::getButtonStyle()
{
	return style;
}

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
}

void Button::OnMouseUp()
{
	if (style == ButtonStyle::CLICK)
	{
		ButtonPressedEvent buttonPressedEvent(this);
		notify(&buttonPressedEvent);
	}
	else if (style == ButtonStyle::TOGGLE)
	{
		toggled = !toggled;
		ButtonToggledEvent buttonToggledEvent(this, toggled);
		notify(&buttonToggledEvent);
	}
}