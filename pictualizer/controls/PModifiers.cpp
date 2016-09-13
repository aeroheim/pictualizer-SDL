#pragma once

#include "PModifiers.h"
#include "../config/PGlobals.h"
#include <cmath>
#include <algorithm>

PModifier::~PModifier()
{
}

void PModifier::setCurrentValue(float currentValue)
{
	if (_startValue < _endValue)
	{
		_currentValue = std::max(currentValue, _startValue);
		_currentValue = std::min(_currentValue, _endValue);
	}
	else if (_startValue >= _endValue)
	{
		_currentValue = std::min(currentValue, _startValue);
		_currentValue = std::max(currentValue, _endValue);
	}
}

float PModifier::getCurrentValue() const
{
	return _currentValue;
}

void PModifier::setDuration(int seconds)
{
	_durationInSeconds = seconds;
}

float PModifier::getDuration() const
{
	return _durationInSeconds;
}

void PModifier::setRange(float startValue, float endValue)
{
	_startValue = startValue;
	_endValue = endValue;
}

void PModifier::setStartValue(float startValue)
{
	_startValue = startValue;
}

void PModifier::setEndValue(float endValue)
{
	_endValue = endValue;
}

float PModifier::getStartValue() const
{
	return _startValue;
}

float PModifier::getEndValue() const
{
	return _endValue;
}

bool PModifier::isFinished() const
{
	return _currentValue == _endValue;
}

void PModifier::invert()
{
	setRange(_endValue, _startValue);
}


PModifier::PModifier(int durationInSeconds, float startValue, float endValue) : _durationInSeconds(durationInSeconds), _startValue(startValue), _endValue(endValue), _currentValue(startValue)
{
}

LinearModifier::LinearModifier(int durationInSeconds, float startValue, float endValue) : PModifier(durationInSeconds, startValue, endValue)
{
	setDelta();
}

float LinearModifier::getNextValue()
{
	if (!isFinished())
	{
		setCurrentValue(_currentValue - _delta);
		return _currentValue;
	}
	
	return _currentValue;
}

void LinearModifier::setDuration(int seconds)
{
	PModifier::setDuration(seconds);
	setDelta();
}

void LinearModifier::setRange(float startValue, float endValue)
{
	PModifier::setRange(startValue, endValue);
	setDelta();
}

void LinearModifier::setStartValue(float startValue)
{
	PModifier::setStartValue(startValue);
	setDelta();
}

void LinearModifier::setEndValue(float endValue)
{
	PModifier::setEndValue(endValue);
	setDelta();
}

void LinearModifier::setDelta()
{
	_delta = (_startValue - _endValue) / (_durationInSeconds * PConstants::FRAMERATE);
}

SmoothModifier::SmoothModifier(int durationInSeconds, float startValue, float endValue) : PModifier(durationInSeconds, startValue, endValue)
{
	setDomain();
}

float SmoothModifier::getNextValue()
{
	if (!isFinished())
	{
		float valueRange = _startValue - _endValue;

		_currentPoint = _currentPoint + 1 > _domain ? _currentPoint : _currentPoint + 1;

		// We produce a scalar to scale against pi, shifted by 1 depending on whether we're curving upwards or downwards.
		float pointScalar = _startValue > _endValue ? (_currentPoint / _domain) : (_currentPoint / _domain) + 1;

		// The following function produces a smooth cosine curve: (cos(x * pi) + 1) / 2)
		float valueScalar = (std::cos(pointScalar * M_PI) + 1) / 2;
		setCurrentValue(valueRange * valueScalar);
		return _currentValue;
	}

	return _currentValue;
}

void SmoothModifier::setDuration(int seconds)
{
	PModifier::setDuration(seconds);
	setDomain();
}

void SmoothModifier::setDomain()
{
	_domain = _durationInSeconds * PConstants::FRAMERATE;
}

