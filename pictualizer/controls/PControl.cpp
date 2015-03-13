#include "PControl.h"

PControl::PControl(float x, float y, float w, float h) : x(x), y(y), w(w), h(h), r(-1), g(-1), b(-1), a(255), aDelta(0), fadeState(PControlFadeState::NONE) {}

PControl::~PControl() {};

void PControl::setX(float x)
{
	this->x = x;
}

void PControl::setY(float y)
{
	this->y = y;
}

void PControl::setWidth(float w)
{
	this->w = w;
}

void PControl::setHeight(float h)
{
	this->h = h;
}

float PControl::getX()
{
	return x;
}

float PControl::getY()
{
	return y;
}

float PControl::getWidth()
{
	return w;
}

float PControl::getHeight()
{
	return h;
}

int PControl::getRoundedX()
{
	return (int) std::round(x);
}

int PControl::getRoundedY()
{
	return (int) std::round(y);
}

int PControl::getRoundedWidth()
{
	return (int) std::round(w);
}

int PControl::getRoundedHeight()
{
	return (int) std::round(h);
}

bool PControl::mouseInside(int x, int y)
{
	return ((x >= getRoundedX() && x <= getRoundedX() + getRoundedWidth()) && (y >= getRoundedY() && y <= getRoundedY() + getRoundedHeight()));
}

void PControl::moveTo(float x, float y)
{
	setX(x);
	setY(y);
}

void PControl::resize(float w, float h)
{
	setWidth(w);
	setHeight(h);
}

void PControl::setColor(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

PFloatColor PControl::getColor()
{
	PFloatColor color{ r, g, b };
	return color;
}

PIntColor PControl::getRoundedColor()
{
	PIntColor color{ (Uint8) std::round(r), (Uint8) std::round(g), (Uint8) std::round(b) };
	return color;
}

void PControl::setAlpha(float a)
{
	assert(a >= 0 && a <= 255);

	this->a = a;

	switch (fadeState)
	{
		case PControlFadeState::FADEIN:
			if (a >= 255.0f)
				fadeState = PControlFadeState::NONE;
			break;
		case PControlFadeState::FADEOUT:
			if (a <= 0.0f)
				fadeState = PControlFadeState::NONE;
			break;
		case PControlFadeState::NONE:
			break;
	}
}

float PControl::getAlpha()
{
	return a;
}

Uint8 PControl::getRoundedAlpha()
{
	return (Uint8) std::round(a);
}

void PControl::setFadeState(PControlFadeState s)
{
	fadeState = s;
}

PControlFadeState PControl::getFadeState()
{
	return fadeState;
}

void PControl::setFadeDelta(float delta)
{
	assert(delta >= 0);

	aDelta = delta;
}

float PControl::getFadeDelta()
{
	return aDelta;
}

void PControl::draw(SDL_Renderer* ren = nullptr)
{
	switch (fadeState)
	{
		case PControlFadeState::FADEIN:
			setAlpha(a + aDelta < 255 ? a + aDelta : 255.0f); 
			break;
		case PControlFadeState::FADEOUT:
			setAlpha(a - aDelta > 0 ? a - aDelta : 0.0f);
			break;
		case PControlFadeState::NONE:
			break;
	}
}



