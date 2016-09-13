#pragma once

#include "PFadeable.h"
#include "../config/PGlobals.h"
#include <algorithm>
#include <stdexcept>

PFadeable::~PFadeable()
{
}

void PFadeable::setAlpha(float alpha)
{
	_currentAlpha = std::min(std::max(_minAlpha, alpha), _maxAlpha);
	resetFader();
}

float PFadeable::getAlpha() const
{
	return _currentAlpha;
}

Uint8 PFadeable::getRoundedAlpha() const
{
	return std::round(_currentAlpha);
}

void PFadeable::setAlphaRange(float minAlpha, float maxAlpha)
{
	if (minAlpha < PConstants::MIN_ALPHA || maxAlpha > PConstants::MAX_ALPHA || minAlpha > maxAlpha)
		throw std::invalid_argument("Received invalid alpha range.");

	_minAlpha = minAlpha;
	_maxAlpha = maxAlpha;
	_currentAlpha = std::min(std::max(_minAlpha, _currentAlpha), _maxAlpha);
	resetFader();
}

float PFadeable::getMinAlpha() const
{
	return _minAlpha;
}

float PFadeable::getMaxAlpha() const
{
	return _maxAlpha;
}

void PFadeable::setFadeState(PFadeState fadeState)
{
	_fadeState = fadeState;
	resetFader();
}

void PFadeable::setFader(PModifier* fader)
{
	_fader.reset(fader);
	resetFader();
}

PFadeState PFadeable::getFadeState() const
{
	return _fadeState;
}

void PFadeable::setFadeDuration(float seconds)
{
	_fadeDurationInSeconds = seconds;
	resetFader();
}

float PFadeable::getFadeDuration() const
{
	return _fadeDurationInSeconds;
}

void PFadeable::fade()
{
	if (_fadeState != PFadeState::NONE)
	{
		PModifier* fader;
		if ((fader = _fader.get()) != nullptr)
		{
			// Get next alpha value from the fader.
			_currentAlpha = std::min(std::max(_minAlpha, fader->getNextValue()), _maxAlpha);

			// Finished fading, reset fade state.
			if (fader->isFinished())
			{
				PFadeState previousFadeState = _fadeState;
				_fadeState = PFadeState::NONE;

				// Call fade finished handler.
				OnFadeFinished(previousFadeState);
			}
		}
	}
}

PFadeable::PFadeable() : _fadeState(PFadeState::NONE), _minAlpha(PConstants::MIN_ALPHA), _maxAlpha(PConstants::MAX_ALPHA), _currentAlpha(255), _fadeDurationInSeconds(0)
{
}

void PFadeable::OnFadeFinished(PFadeState previousFadeState)
{
}

void PFadeable::resetFader() const
{
	PModifier* fader;
	if ((fader = _fader.get()) != nullptr)
	{
		switch (_fadeState)
		{
			case PFadeState::FADEIN:
				fader->setRange(_currentAlpha, _maxAlpha);
				fader->setDuration(_fadeDurationInSeconds);
				break;
			case PFadeState::FADEOUT:
				fader->setRange(_currentAlpha, _minAlpha);
				fader->setDuration(_fadeDurationInSeconds);
				break;
			default:
				break;
		}
	}
}
