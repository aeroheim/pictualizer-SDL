#include "ImageButton.h"

ImageButton::ImageButton(SDL_Texture* image, float x, float y, float w, float h) :
	Button(x, y, w, h),
	image(image, x, y, w, h),
	mouseOver(false) {}

ImageButton::~ImageButton()
{
	image.freeImage();
}

void ImageButton::setImage(SDL_Texture* image)
{
	this->image.setImage(image);
}

void ImageButton::setX(float x)
{
	PControl::setX(x);
	image.setX(x);
}

void ImageButton::setY(float y)
{
	PControl::setY(y);
	image.setY(y);
}

void ImageButton::setWidth(float w)
{
	PControl::setWidth(w);
	image.setWidth(w);
}

void ImageButton::setHeight(float h)
{
	PControl::setHeight(h);
	image.setHeight(h);
}

void ImageButton::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
	image.setColor(r, g, b);
}

void ImageButton::setTint(float t)
{
	PControl::setTint(t);
	image.setTint(t);
}

void ImageButton::setBaseTint(float t)
{
	PControl::setBaseTint(t);
	image.setBaseTint(t);
}

void ImageButton::setFocusTint(float t)
{
	PControl::setFocusTint(t);
	image.setFocusTint(t);
}

void ImageButton::setTintState(PControlTintState s)
{
	PControl::setTintState(s);
	image.setTintState(s);
}

void ImageButton::setTintDelta(float delta)
{
	PControl::setTintDelta(delta);
	image.setTintDelta(delta);
}

void ImageButton::setAlpha(float a)
{
	PControl::setAlpha(a);
	image.setAlpha(a);
}

void ImageButton::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);
	image.setMinAlpha(a);
}

void ImageButton::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);
	image.setMaxAlpha(a);
}

void ImageButton::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);
	image.setFadeState(s);
}

void ImageButton::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);
	image.setFadeDelta(delta);
}

void ImageButton::draw(SDL_Renderer* ren)
{
	image.draw(ren);
	PControl::draw(nullptr);
}

void ImageButton::handleEvent(Event* e)
{
	if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		OnMouseMotion(mouseMotionEvent);

	Button::handleEvent(e);
}

void ImageButton::OnMouseMotion(MouseMotionEvent* e)
{
	bool mouseOverButton = mouseInside(e->x, e->y);

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
