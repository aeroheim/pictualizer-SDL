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

PControl& PControl::operator=(const PControl& other)
{
	setX(other.getX());
	setY(other.getY());
	setWidth(other.getWidth());
	setHeight(other.getHeight());

	PFloatColor color = other.getColor();
	setColor(color.r, color.g, color.b);

	setTint(other.getTint());
	setBaseTint(other.getBaseTint());
	setFocusTint(other.getFocusTint());
	setTintState(other.getTintState());
	setTintDelta(other.getTintDelta());

	setAlpha(other.getAlpha());
	setMinAlpha(other.getMinAlpha());
	setMaxAlpha(other.getMaxAlpha());
	setFadeState(other.getFadeState());
	setFadeDelta(other.getFadeDelta());

	return *this;
}

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

float PControl::getX() const
{
	return x;
}

float PControl::getY() const
{
	return y;
}

float PControl::getWidth() const
{
	return w;
}

float PControl::getHeight() const
{
	return h;
}

int PControl::getRoundedX() const
{
	return (int) std::round(x);
}

int PControl::getRoundedY() const
{
	return (int) std::round(y);
}

int PControl::getRoundedWidth() const
{
	return (int) std::round(w);
}

int PControl::getRoundedHeight() const
{
	return (int) std::round(h);
}

bool PControl::mouseInside(int x, int y) const
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

PFloatColor PControl::getColor() const
{
	PFloatColor color{ r, g, b };
	return color;
}

PIntColor PControl::getRoundedColor() const
{
	PIntColor color{ (Uint8) std::round(r), (Uint8) std::round(g), (Uint8) std::round(b) };
	return color;
}

void PControl::setTint(float t)
{
	assert(t >= 0 && t <= 255);

	tint = t;

	if (r != t && g != t && b != t)
		setColor(t, t, t);

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

float PControl::getTint() const
{
	return tint;
}

Uint8 PControl::getRoundedTint() const
{
	return (Uint8) std::round(tint);
}

void PControl::setBaseTint(float t)
{
	assert(t <= focusTint);

	baseTint = t;
}

float PControl::getBaseTint() const
{
	return baseTint;
}

void PControl::setFocusTint(float t)
{
	assert(t >= baseTint);

	focusTint = t;
}

float PControl::getFocusTint() const
{
	return focusTint;
}

void PControl::setTintState(PControlTintState s)
{
	tintState = s;

	if (tintState == PControlTintState::NONE)
		setTint(baseTint);
}

PControlTintState PControl::getTintState() const
{
	return tintState;
}

void PControl::setTintDelta(float delta)
{
	assert(delta >= 0);

	tintDelta = delta;
}

float PControl::getTintDelta() const
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

float PControl::getAlpha() const
{
	return alpha;
}

Uint8 PControl::getRoundedAlpha() const
{
	return (Uint8) std::round(alpha);
}

void PControl::setMinAlpha(float a)
{
	assert(a <= maxAlpha);

	minAlpha = a;
}

float PControl::getMinAlpha() const
{
	return minAlpha;
}

void PControl::setMaxAlpha(float a)
{
	assert(a >= minAlpha);

	maxAlpha = a;
}

float PControl::getMaxAlpha() const
{
	return maxAlpha;
}

void PControl::setFadeState(PControlFadeState s)
{
	fadeState = s;
}

PControlFadeState PControl::getFadeState() const
{
	return fadeState;
}

void PControl::setFadeDelta(float delta)
{
	assert(delta >= 0);

	alphaDelta = delta;
}

float PControl::getFadeDelta() const
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

	/*
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY(), getRoundedX() + getRoundedWidth(), getRoundedY());
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY(), getRoundedX(), getRoundedY() + getRoundedHeight());
	SDL_RenderDrawLine(ren, getRoundedX() + getRoundedWidth(), getRoundedY(), getRoundedX() + getRoundedWidth(), getRoundedY() + getRoundedHeight());
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY() + getRoundedHeight(), getRoundedX() + getRoundedWidth(), getRoundedY() + getRoundedHeight());

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawPoint(ren, getRoundedX(), getRoundedY());
	SDL_RenderDrawPoint(ren, getRoundedX() + getRoundedWidth(), getRoundedY());
	SDL_RenderDrawPoint(ren, getRoundedX(), getRoundedY() + getRoundedHeight());
	SDL_RenderDrawPoint(ren, getRoundedX() + getRoundedWidth(), getRoundedY() + getRoundedHeight());
	*/
}



