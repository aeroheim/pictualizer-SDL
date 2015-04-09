#include "GridPanelCell.h"

GridPanelCell::GridPanelCell(float x, float y, float w, float h) : PControl(x, y, w, h)
{
	element = nullptr;

	for (int i = 0; i < 4; i++)
		padding[i] = 0;
}

GridPanelCell::GridPanelCell(PControl* e, float x, float y, float w, float h) : PControl(x, y, w, h)
{
	if (element)
	{
		element = e;
		e->setX(x);
		e->setY(y);
		e->setWidth(w);
		e->setHeight(h);
	}
	else
		element = nullptr;

	for (int i = 0; i < 4; i++)
		padding[i] = 0;
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

PControl* GridPanelCell::getElement()
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

void GridPanelCell::setTint(float t)
{
	PControl::setTint(t);

	if (element)
		element->setTint(t);
}

void GridPanelCell::setBaseTint(float t)
{
	PControl::setBaseTint(t);

	if (element)
		element->setBaseTint(t);
}

void GridPanelCell::setFocusTint(float t)
{
	PControl::setFocusTint(t);

	if (element)
		element->setFocusTint(t);
}

void GridPanelCell::setTintState(PControlTintState s)
{
	PControl::setTintState(s);

	if (element)
		element->setTintState(s);
}

void GridPanelCell::setTintDelta(float delta)
{
	PControl::setTintDelta(delta);

	if (element)
		element->setTintDelta(delta);
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

void GridPanelCell::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);

	if (element)
		element->setFadeDelta(delta);
}

void GridPanelCell::draw(SDL_Renderer* ren)
{
	if (element)
		element->draw(ren);

	PControl::draw(ren);
}




