#pragma once

#include "PControl.h"
#include <assert.h>
#include <algorithm>
#include "../config/PGlobals.h"

using namespace PGlobals;

PControl::PControl(float x, float y, float w, float h) : 
	x(x), y(y), w(w), h(h), 
	minW(0), minH(0), maxW(PConstants::PCONTROL_DEFAULT_MAXDIM), maxH(PConstants::PCONTROL_DEFAULT_MAXDIM),
	colorState(PControlColorState::NONE), colorStyle(PControlColorStyle::LINEAR),
	baseColor({ PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB }), 
	focusColor({ PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB }),
	color({ PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB, PConstants::PCONTROL_MAX_RGB }),
	colorSpeed(0), rDelta(0), gDelta(0), bDelta(0),
	fadeState(PControlFadeState::NONE), fadeStyle(PControlFadeStyle::LINEAR),
	minAlpha(PConstants::PCONTROL_MIN_ALPHA), maxAlpha(PConstants::PCONTROL_MAX_ALPHA), alpha(PConstants::PCONTROL_MAX_ALPHA), fadeSpeed(0), fadeDelta(0)
{}

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
	if (w >= minW && w <= maxW)
		this->w = w;
}

void PControl::setHeight(float h)
{
	if (h >= minH && h <= maxH)
		this->h = h;
}

void PControl::setMinWidth(float minWidth)
{
	assert(minWidth >= 0 && minWidth <= maxW);

	minW = minWidth;

	if (w < minW)
		setWidth(minW);
}

void PControl::setMinHeight(float minHeight)
{
	assert(minHeight >= 0 && minHeight <= maxH);

	minH = minHeight;

	if (h < minH)
		setHeight(minH);
}

void PControl::setMaxWidth(float maxWidth)
{
	assert(maxWidth >= minW);

	maxW = maxWidth;

	if (w > maxW)
		setWidth(maxW);
}

void PControl::setMaxHeight(float maxHeight)
{
	assert(maxHeight >= maxH);

	maxH = maxHeight;

	if (h > maxH)
		setHeight(maxH);
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

float PControl::getMinWidth() const
{
	return minW;
}

float PControl::getMinHeight() const
{
	return minH;
}

float PControl::getMaxWidth() const
{
	return maxW;
}

float PControl::getMaxHeight() const
{
	return maxH;
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
	this->x = x;
	this->y = y;
}

void PControl::resize(float w, float h)
{
	this->w = w;
	this->h = h;
}

void PControl::setColor(float r, float g, float b)
{
	color.r = r;
	color.g = g;
	color.b = b;

	switch (colorState)
	{
		case PControlColorState::BASE:
			if (color.r == baseColor.r && color.g == baseColor.g && color.b == baseColor.b)
				colorState = PControlColorState::NONE;
			break;
		case PControlColorState::FOCUS:
			if (color.r == focusColor.r && color.g == focusColor.g && color.b == focusColor.b)
				colorState = PControlColorState::NONE;
			break;
		case PControlColorState::NONE:
			break;
	}
}

void PControl::setColor(const PFloatColor& c)
{
	setColor(c.r, c.g, c.b);
}

void PControl::setColor(const PIntColor& c)
{
	setColor(c.r, c.g, c.b);
}

const PFloatColor& PControl::getColor() const
{
	return color;
}

PIntColor PControl::getRoundedColor() const
{
	PIntColor roundedColor{ (Uint8) std::round(color.r), (Uint8) std::round(color.g), (Uint8) std::round(color.b) };
	return roundedColor;
}

void PControl::setBaseColor(float r, float g, float b)
{
	assert(r >= PConstants::PCONTROL_MIN_RGB && r <= PConstants::PCONTROL_MAX_RGB);
	assert(g >= PConstants::PCONTROL_MIN_RGB && g <= PConstants::PCONTROL_MAX_RGB);
	assert(b >= PConstants::PCONTROL_MIN_RGB && b <= PConstants::PCONTROL_MAX_RGB);

	baseColor.r = r;
	baseColor.g = g;
	baseColor.b = b;

	setColorDelta();
}

void PControl::setBaseColor(const PFloatColor& c)
{
	setBaseColor(c.r, c.g, c.b);
}

void PControl::setBaseColor(const PIntColor& c)
{
	setBaseColor(c.r, c.g, c.b);
}

const PFloatColor& PControl::getBaseColor() const
{
	return baseColor;
}

PIntColor PControl::getRoundedBaseColor() const
{
	PIntColor roundedBaseColor{ (Uint8)std::round(baseColor.r), (Uint8)std::round(baseColor.g), (Uint8)std::round(baseColor.b) };
	return roundedBaseColor;
}

void PControl::setFocusColor(float r, float g, float b)
{
	assert(r >= PConstants::PCONTROL_MIN_RGB && r <= PConstants::PCONTROL_MAX_RGB);
	assert(g >= PConstants::PCONTROL_MIN_RGB && g <= PConstants::PCONTROL_MAX_RGB);
	assert(b >= PConstants::PCONTROL_MIN_RGB && b <= PConstants::PCONTROL_MAX_RGB);

	focusColor.r = r;
	focusColor.g = g;
	focusColor.b = b;

	setColorDelta();
}

void PControl::setFocusColor(const PFloatColor& c)
{
	setFocusColor(c.r, c.g, c.b);
}

void PControl::setFocusColor(const PIntColor& c)
{
	setFocusColor(c.r, c.g, c.b);
}

const PFloatColor& PControl::getFocusColor() const
{
	return focusColor;
}

PIntColor PControl::getRoundedFocusColor() const
{
	PIntColor roundedFocusColor{ (Uint8)std::round(focusColor.r), (Uint8)std::round(focusColor.g), (Uint8)std::round(focusColor.b) };
	return roundedFocusColor;
}

void PControl::setColorState(PControlColorState s)
{
	colorState = s;

	if (colorState == PControlColorState::NONE)
		setColor(baseColor);
}

void PControl::setColorStyle(PControlColorStyle s)
{
	colorStyle = s;
}

PControlColorState PControl::getColorState() const
{
	return colorState;
}

PControlColorStyle PControl::getColorStyle() const
{
	return colorStyle;
}

void PControl::setColorSpeed(float seconds)
{
	assert(seconds >= 0);

	colorSpeed = seconds;
	setColorDelta();
}

float PControl::getColorSpeed() const
{
	return colorSpeed;
}

void PControl::setAlpha(float a)
{
	assert(a >= PConstants::PCONTROL_MIN_ALPHA && a <= PConstants::PCONTROL_MAX_ALPHA);

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
	assert(a >= PConstants::PCONTROL_MIN_ALPHA && a <= maxAlpha);

	minAlpha = a;
	setFadeDelta();
}

float PControl::getMinAlpha() const
{
	return minAlpha;
}

void PControl::setMaxAlpha(float a)
{
	assert(a <= PConstants::PCONTROL_MAX_ALPHA && a >= minAlpha);

	maxAlpha = a;
	setFadeDelta();
}

float PControl::getMaxAlpha() const
{
	return maxAlpha;
}

void PControl::setFadeState(PControlFadeState s)
{
	fadeState = s;
}

void PControl::setFadeStyle(PControlFadeStyle s)
{
	fadeStyle = s;
}

PControlFadeState PControl::getFadeState() const
{
	return fadeState;
}

PControlFadeStyle PControl::getFadeStyle() const
{
	return fadeStyle;
}

void PControl::setFadeSpeed(float seconds)
{
	assert(seconds >= 0);

	fadeSpeed = seconds;
	setFadeDelta();
}

float PControl::getFadeSpeed() const
{
	return fadeSpeed;
}

void PControl::setFadeDelta()
{
	fadeDelta = (maxAlpha - minAlpha) / (PConstants::P_FRAMERATE * fadeSpeed);
}

void PControl::setColorDelta()
{
	float framesToFade = PConstants::P_FRAMERATE * colorSpeed;

	rDelta = (focusColor.r - baseColor.r) / framesToFade;
	gDelta = (focusColor.g - baseColor.g) / framesToFade;
	bDelta = (focusColor.b - baseColor.b) / framesToFade;
}

void PControl::draw(SDL_Renderer* ren = nullptr)
{
	switch (fadeState)
	{
		case PControlFadeState::FADEIN:
			setAlpha(alpha + fadeDelta < maxAlpha ? alpha + fadeDelta : maxAlpha); 
			break;
		case PControlFadeState::FADEOUT:
			setAlpha(alpha - fadeDelta > minAlpha ? alpha - fadeDelta : minAlpha);
			break;
		case PControlFadeState::NONE:
			break;
	}

	switch (colorState)
	{
		case PControlColorState::BASE:
			setColor(focusColor.r > baseColor.r ? (color.r - rDelta > baseColor.r ? color.r - rDelta : baseColor.r) : (color.r - rDelta < baseColor.r ? color.r - rDelta : baseColor.r),
						focusColor.g > baseColor.g ? (color.g - gDelta > baseColor.g ? color.g - gDelta : baseColor.g) : (color.g - gDelta < baseColor.g ? color.g - gDelta : baseColor.g),
						focusColor.b > baseColor.b ? (color.b - bDelta > baseColor.b ? color.b - bDelta : baseColor.b) : (color.b - bDelta < baseColor.b ? color.b - bDelta : baseColor.b));
			break;
		case PControlColorState::FOCUS:
			setColor(focusColor.r > baseColor.r ? (color.r + rDelta < focusColor.r ? color.r + rDelta : focusColor.r) : (color.r + rDelta > focusColor.r ? color.r + rDelta : focusColor.r),
						focusColor.g > baseColor.g ? (color.g + gDelta < focusColor.g ? color.g + gDelta : focusColor.g) : (color.g + gDelta > focusColor.g ? color.g + gDelta : focusColor.g),
						focusColor.b > baseColor.b ? (color.b + bDelta < focusColor.b ? color.b + bDelta : focusColor.b) : (color.b + bDelta > focusColor.b ? color.b + bDelta : focusColor.b));
			break;
		case PControlColorState::NONE:
			break;
	}

	/*
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY(), getRoundedX() + getRoundedWidth(), getRoundedY());
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY(), getRoundedX(), getRoundedY() + getRoundedHeight());
	SDL_RenderDrawLine(ren, getRoundedX() + getRoundedWidth(), getRoundedY(), getRoundedX() + getRoundedWidth(), getRoundedY() + getRoundedHeight());
	SDL_RenderDrawLine(ren, getRoundedX(), getRoundedY() + getRoundedHeight(), getRoundedX() + getRoundedWidth(), getRoundedY() + getRoundedHeight());

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	*/
}



