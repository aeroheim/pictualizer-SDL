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
	texture.freeTexture();
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

void PTextureButton::setTint(float t)
{
	PControl::setTint(t);
	texture.setTint(t);
}

void PTextureButton::setBaseTint(float t)
{
	PControl::setBaseTint(t);
	texture.setBaseTint(t);
}

void PTextureButton::setFocusTint(float t)
{
	PControl::setFocusTint(t);
	texture.setFocusTint(t);
}

void PTextureButton::setTintState(PControlTintState s)
{
	PControl::setTintState(s);
	texture.setTintState(s);
}

void PTextureButton::setTintDelta(float delta)
{
	PControl::setTintDelta(delta);
	texture.setTintDelta(delta);
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

void PTextureButton::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);
	texture.setFadeDelta(delta);
}

void PTextureButton::draw(SDL_Renderer* ren)
{
	texture.draw(ren);
	PControl::draw(nullptr);
}

void PTextureButton::handleEvent(Event* e)
{
	if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		OnMouseMotion(mouseMotionEvent);

	Button::handleEvent(e);
}

void PTextureButton::OnMouseMotion(MouseMotionEvent* e)
{
	bool mouseOverButton = mouseInside(e->x, e->y);

	if (lMouseHeld && getTintState() != PControlTintState::FOCUS)
		setTintState(PControlTintState::FOCUS);
	else
	{
		if (!mouseOver && mouseOverButton)
		{
			mouseOver = true;
			setTintState(PControlTintState::FOCUS);
		}
		else if (!mouseOverButton)
		{
			mouseOver = false;
			setTintState(PControlTintState::BASE);
		}
	}
}
