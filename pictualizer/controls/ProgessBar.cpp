#pragma once

#include "ProgressBar.h"
#include <cassert>

ProgressBar::ProgressBar(SDL_Renderer* ren, float x, float y, float w, float h) :
	PControl(x, y, w, h),
	baseBar(x, y, w, h),
	progressBar(x, y, w, h),
	progress(0),
	maxProgress(0)
{
	SDL_Surface* baseBarSurface = SDL_CreateRGBSurface(0, (int) std::round(w), (int) std::round(h), 32, 0, 0, 0, 0);
	SDL_FillRect(baseBarSurface, NULL, SDL_MapRGBA(baseBarSurface->format, 255, 255, 255, 255));

	SDL_Texture* baseBarTexture = SDL_CreateTextureFromSurface(ren, baseBarSurface);
	SDL_SetTextureBlendMode(baseBarTexture, SDL_BLENDMODE_BLEND);

	SDL_Surface* progressBarSurface = SDL_CreateRGBSurface(0, (int) std::round(w), (int) std::round(h), 32, 0, 0, 0, 0);
	SDL_FillRect(progressBarSurface, NULL, SDL_MapRGBA(progressBarSurface->format, 255, 255, 255, 255));

	SDL_Texture* progressBarTexture = SDL_CreateTextureFromSurface(ren, progressBarSurface);
	SDL_SetTextureBlendMode(progressBarTexture, SDL_BLENDMODE_BLEND);

	baseBar.setImage(Image::makeImageTexture(baseBarTexture));
	progressBar.setImage(Image::makeImageTexture(progressBarTexture));

	SDL_FreeSurface(baseBarSurface);
	SDL_FreeSurface(progressBarSurface);
}

void ProgressBar::setProgress(float units)
{
	assert(units <= maxProgress && units >= 0);

	progress = units;

	progressBar.setWidth(baseBar.getWidth() * getPercentProgress());
}

void ProgressBar::setMaxProgress(float units)
{
	assert(units >= 0);

	maxProgress = units;

	progressBar.setWidth(getPercentProgress() < 1 ? baseBar.getWidth() * getPercentProgress() : baseBar.getWidth());
}

float ProgressBar::getUnitProgress()
{
	return progress;
}

float ProgressBar::getMaxUnitProgress()
{
	return maxProgress;
}

float ProgressBar::getPercentProgress()
{
	return maxProgress != 0 ? progress / maxProgress : 1;
}

void ProgressBar::setX(float x)
{
	PControl::setX(x);
	baseBar.setX(x);
	progressBar.setX(x);
}

void ProgressBar::setY(float y)
{
	PControl::setY(y);
	baseBar.setY(y);
	progressBar.setY(y);
}

void ProgressBar::setWidth(float w)
{
	PControl::setWidth(w);
	baseBar.setWidth(w);
	progressBar.setWidth(w * getPercentProgress());
}

void ProgressBar::setHeight(float h)
{
	PControl::setHeight(h);
	baseBar.setHeight(h);
	progressBar.setHeight(h);
}

void ProgressBar::setColor(float r, float g, float b)
{
	setBarColor(r, g, b);
}

void ProgressBar::setBarColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
	baseBar.setColor(r, g, b);
}

void ProgressBar::setProgressColor(float r, float g, float b)
{
	progressBar.setColor(r, g, b);
}

const PFloatColor& ProgressBar::getBarColor() const
{
	return baseBar.getColor();
}

PIntColor ProgressBar::getRoundedBarColor() const
{
	return baseBar.getRoundedColor();
}

const PFloatColor& ProgressBar::getProgressColor() const
{
	return progressBar.getColor();
}

PIntColor ProgressBar::getRoundedProgressColor() const
{
	return progressBar.getRoundedColor();
}


void ProgressBar::setAlpha(float a)
{
	PControl::setAlpha(a);
	baseBar.setAlpha(a);
	progressBar.setAlpha(a);
}

void ProgressBar::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);
	baseBar.setMinAlpha(a);
	progressBar.setMinAlpha(a);
}

void ProgressBar::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);
	baseBar.setMaxAlpha(a);
	progressBar.setMaxAlpha(a);
}

void ProgressBar::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);
	baseBar.setFadeState(s);
	progressBar.setFadeState(s);
}

void ProgressBar::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);
	baseBar.setFadeStyle(s);
	progressBar.setFadeStyle(s);
}

void ProgressBar::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);
	baseBar.setFadeSpeed(seconds);
	progressBar.setFadeSpeed(seconds);
}

void ProgressBar::draw(SDL_Renderer* ren)
{
	baseBar.draw(ren);
	progressBar.draw(ren);

	PControl::draw(nullptr);
}