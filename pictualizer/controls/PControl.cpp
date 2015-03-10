#include "PControl.h"

PControl::PControl(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {};

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

