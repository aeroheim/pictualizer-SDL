#include "PControl.h"

PControl::PControl(float x, float y, float w, float h) : 
	x(x), y(y), w(w), h(h), 
	r(-1), g(-1), b(-1),
	tintState(PControlTintState::NONE),
	baseTint(255),
	focusTint(255),
	tintDelta(0),
	tint(255),
	fadeState(PControlFadeState::NONE),
	minAlpha(0),
	maxAlpha(255),
	alphaDelta(0),
	alpha(255) {}

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

void PControl::setTint(float t)
{
	assert(t >= 0 && t <= 255);

	tint = t;

	if (r != tint && g != tint && b != tint)
		setColor(tint, tint, tint);

	switch (tintState)
	{
		case PControlTintState::BASE:
			if (tint <= baseTint)
				tintState = PControlTintState::NONE;
			break;
		case PControlTintState::FOCUS:
			if (tint >= focusTint)
				tintState = PControlTintState::NONE;
			break;
		case PControlTintState::NONE:
			break;
	}
}

float PControl::getTint()
{
	return tint;
}

Uint8 PControl::getRoundedTint()
{
	return (Uint8) std::round(tint);
}

void PControl::setBaseTint(float t)
{
	assert(t <= focusTint);

	baseTint = t;
}

float PControl::getBaseTint()
{
	return baseTint;
}

void PControl::setFocusTint(float t)
{
	assert(t >= baseTint);

	focusTint = t;
}

float PControl::getFocusTint()
{
	return focusTint;
}

void PControl::setTintState(PControlTintState s)
{
	tintState = s;

	if (tintState == PControlTintState::NONE)
		setTint(baseTint);
}

PControlTintState PControl::getTintState()
{
	return tintState;
}

void PControl::setTintDelta(float delta)
{
	assert(delta >= 0);

	tintDelta = delta;
}

float PControl::getTintDelta()
{
	return tintDelta;
}

void PControl::setAlpha(float a)
{
	assert(a >= 0 && a <= 255);

	alpha = a;

	switch (fadeState)
	{
		case PControlFadeState::FADEIN:
			if (alpha >= maxAlpha)
				fadeState = PControlFadeState::NONE;
			break;
		case PControlFadeState::FADEOUT:
			if (alpha <= minAlpha)
				fadeState = PControlFadeState::NONE;
			break;
		case PControlFadeState::NONE:
			break;
	}
}

float PControl::getAlpha()
{
	return alpha;
}

Uint8 PControl::getRoundedAlpha()
{
	return (Uint8) std::round(alpha);
}

void PControl::setMinAlpha(float a)
{
	assert(a <= maxAlpha);

	minAlpha = a;
}

float PControl::getMinAlpha()
{
	return minAlpha;
}

void PControl::setMaxAlpha(float a)
{
	assert(a >= minAlpha);

	maxAlpha = a;
}

float PControl::getMaxAlpha()
{
	return maxAlpha;
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

	alphaDelta = delta;
}

float PControl::getFadeDelta()
{
	return alphaDelta;
}

void PControl::draw(SDL_Renderer* ren = nullptr)
{
	switch (fadeState)
	{
		case PControlFadeState::FADEIN:
			setAlpha(alpha + alphaDelta < maxAlpha ? alpha + alphaDelta : maxAlpha); 
			break;
		case PControlFadeState::FADEOUT:
			setAlpha(alpha - alphaDelta > minAlpha ? alpha - alphaDelta : minAlpha);
			break;
		case PControlFadeState::NONE:
			break;
	}

	switch (tintState)
	{
		case PControlTintState::BASE:
			setTint(tint - tintDelta > baseTint ? tint - tintDelta : baseTint);
			break;
		case PControlTintState::FOCUS:
			setTint(tint + tintDelta < focusTint ? tint + tintDelta : focusTint);
			break;
		case PControlTintState::NONE:
			break;
	}
}



