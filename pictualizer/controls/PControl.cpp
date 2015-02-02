#include "PControl.h"

PControl::PControl(int x, int y, int w, int h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void PControl::setX(int x)
{
	this->x = x;
}

void PControl::setY(int y)
{
	this->y = y;
}

void PControl::setWidth(int w)
{
	this->w = w;
}

void PControl::setHeight(int h)
{
	this->h = h;
}

int PControl::getX()
{
	return x;
}

int PControl::getY()
{
	return y;
}

int PControl::getWidth()
{
	return w;
}

int PControl::getHeight()
{
	return h;
}

void PControl::moveTo(int x, int y)
{
	this->x = x;
	this->y = y;
}

void PControl::resize(int w, int h)
{
	this->w = w;
	this->h = h;
}

