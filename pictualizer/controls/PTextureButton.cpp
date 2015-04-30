#include "PTextureButton.h"

PTextureButton::PTextureButton(PTextureType textureType, float x, float y, float w, float h) :
	Button(x, y, w, h),
	texture(textureType, x, y, w, h),
	mouseOver(false) {}

PTextureButton::~PTextureButton()
{
	texture.freeTexture();
}

void PTextureButton::setTexture(PTextureType textureType)
{
	texture.setTexture(textureType);
}

void PTextureButton::setX(float x)
{
	PControl::setX(x);
	texture.setX(x);
}

void PTextureButton::setY(float y)
{
	PControl::setY(y);
	texture.setY(y);
}

void PTextureButton::setWidth(float w)
{
	PControl::setWidth(w);
	texture.setWidth(w);
}

void PTextureButton::setHeight(float h)
{
	PControl::setHeight(h);
	texture.setHeight(h);
}

void PTextureButton::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
	texture.setColor(r, g, b);
}

void PTextureButton::setBaseColor(float r, float g, float b)
{
	PControl::setBaseColor(r, g, b);
	texture.setBaseColor(r, g, b);
}

void PTextureButton::setFocusColor(float r, float g, float b)
{
	PControl::setFocusColor(r, g, b);
	texture.setFocusColor(r, g, b);
}

void PTextureButton::setColorState(PControlColorState s)
{
	PControl::setColorState(s);
	texture.setColorState(s);
}

void PTextureButton::setColorStyle(PControlColorStyle s)
{
	PControl::setColorStyle(s);
	texture.setColorStyle(s);
}

void PTextureButton::setColorSpeed(float seconds)
{
	PControl::setColorSpeed(seconds);
	texture.setColorSpeed(seconds);
}

void PTextureButton::setAlpha(float a)
{
	PControl::setAlpha(a);
	texture.setAlpha(a);
}

void PTextureButton::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);
	texture.setMinAlpha(a);
}

void PTextureButton::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);
	texture.setMaxAlpha(a);
}

void PTextureButton::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);
	texture.setFadeState(s);
}

void PTextureButton::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);
	texture.setFadeStyle(s);
}

void PTextureButton::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);
	texture.setFadeSpeed(seconds);
}

void PTextureButton::draw(SDL_Renderer* ren)
{
	texture.draw(ren);
	PControl::draw(nullptr);
}

void PTextureButton::handleEvent(Event* e)
{
	Button::handleEvent(e);
	
	if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
		OnMouseDown(mouseDownEvent);
	else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
		OnMouseUp(mouseUpEvent);
	else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		OnMouseMotion(mouseMotionEvent);
}

void PTextureButton::OnMouseDown(MouseDownEvent* e)
{
	bool mouseOverButton = mouseInside(e->x, e->y);

	if (mouseOverButton)
		setColorState(PControlColorState::FOCUS);
}

void PTextureButton::OnMouseUp(MouseUpEvent* e)
{
	if (!isToggled())
		setColorState(PControlColorState::BASE);
}

void PTextureButton::OnMouseMotion(MouseMotionEvent* e)
{
	bool mouseOverButton = mouseInside(e->x, e->y);

	if (lMouseHeld && getColorState() != PControlColorState::FOCUS)
		setColorState(PControlColorState::FOCUS);
	else if (!isToggled())
	{
		if (!mouseOver && mouseOverButton)
		{
			mouseOver = true;
			setColorState(PControlColorState::FOCUS);
		}
		else if (!mouseOverButton)
		{
			mouseOver = false;
			setColorState(PControlColorState::BASE);
		}
	}
}
