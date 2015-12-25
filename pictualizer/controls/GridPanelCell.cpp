#pragma once

#include "GridPanelCell.h"
#include <cassert>

GridPanelCell::GridPanelCell(float x, float y, float w, float h) : 
	PControl(x, y, w, h),
	view(*this),
	element(nullptr)
{
	for (int i = 0; i < 4; i++)
		padding[i] = 0;
}

GridPanelCell::GridPanelCell(const GridPanelCell& other) :
	PControl(other),
	view(*this),
	element(other.element)
{
	for (int i = 0; i < 4; i++)
		padding[i] = other.padding[i];
}

GridPanelCellView& GridPanelCell::getView()
{
	return view;
}

void GridPanelCell::setElement(PControl* e)
{
	element = e;

	if (element)
	{
		setX(getX());
		setY(getY());
		setWidth(getWidth());
		setHeight(getHeight());
	}
}

PControl* GridPanelCell::getElement() const
{
	return element;
}

void GridPanelCell::setPadding(float l, float u, float r, float d)
{
	assert(l >= 0);
	assert(u >= 0);
	assert(r >= 0);
	assert(d >= 0);

	assert(l + r < 1);
	assert(u + d < 1);

	padding[0] = l;
	padding[1] = u;
	padding[2] = r;
	padding[3] = d;

	setX(getX());
	setY(getY());
	setWidth(getWidth());
	setHeight(getHeight());
}

std::vector<float> GridPanelCell::getPadding() const
{
	std::vector<float> p;

	for (int i = 0; i < 4; i++)
		p.push_back(padding[i]);

	return p;
}

void GridPanelCell::setX(float x)
{
	if (element)
		element->setX(x + (getWidth() * padding[0]));

	PControl::setX(x);
}

void GridPanelCell::setY(float y)
{
	if (element)
		element->setY(y + (getHeight() * padding[1]));

	PControl::setY(y);
}

void GridPanelCell::setWidth(float w)
{
	assert(w >= 0);

	if (element)
		element->setWidth((w - (w * padding[2])) - (w * padding[0]));

	PControl::setWidth(w);
}

void GridPanelCell::setHeight(float h)
{
	assert(h >= 0);

	if (element)
		element->setHeight((h - (h * padding[3])) - (h * padding[1]));

	PControl::setHeight(h);
}

void GridPanelCell::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (element)
		element->setColor(r, g, b);
}

void GridPanelCell::setBaseColor(float r, float g, float b)
{
	PControl::setBaseColor(r, g, b);

	if (element)
		element->setBaseColor(r, g, b);
}

void GridPanelCell::setFocusColor(float r, float g, float b)
{
	PControl::setFocusColor(r, g, b);

	if (element)
		element->setFocusColor(r, g, b);
}

void GridPanelCell::setColorState(PControlColorState s)
{
	PControl::setColorState(s);

	if (element)
		element->setColorState(s);
}

void GridPanelCell::setColorStyle(PControlColorStyle s)
{
	PControl::setColorStyle(s);

	if (element)
		element->setColorStyle(s);
}

void GridPanelCell::setColorSpeed(float seconds)
{
	PControl::setColorSpeed(seconds);

	if (element)
		element->setColorSpeed(seconds);
}

void GridPanelCell::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (element)
		element->setAlpha(a);
}

void GridPanelCell::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);

	if (element)
		element->setMinAlpha(a);
}

void GridPanelCell::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);

	if (element)
		element->setMaxAlpha(a);
}

void GridPanelCell::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);

	if (element)
		element->setFadeState(s);
}

void GridPanelCell::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);

	if (element)
		element->setFadeStyle(s);
}

void GridPanelCell::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);

	if (element)
		element->setFadeSpeed(seconds);
}

void GridPanelCell::draw(SDL_Renderer* ren)
{
	if (element)
		element->draw(ren);

	PControl::draw(ren);
}
