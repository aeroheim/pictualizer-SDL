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

/*
 *  TODO: Disallow negative padding and prevent width/height from being reduced to zero or below because of padding.
 */
void GridPanelCell::setPadding(int l, int u, int r, int d)
{
	assert(l >= 0);
	assert(u >= 0);
	assert(r >= 0);
	assert(d >= 0);

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
		element->setX(x + padding[0]);

	PControl::setX(x);
}

void GridPanelCell::setY(float y)
{
	if (element)
		element->setY(y + padding[1]);

	PControl::setY(y);
}

void GridPanelCell::setWidth(float w)
{
	assert(w >= 0);

	if (element)
		element->setWidth(w - padding[2] - padding[0]);

	PControl::setWidth(w);
}

void GridPanelCell::setHeight(float h)
{
	assert(h >= 0);

	if (element)
		element->setHeight(h - padding[3] - padding[1]);

	PControl::setHeight(h);
}

void GridPanelCell::draw(SDL_Renderer* ren)
{
	if (element)
		element->draw(ren);

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
}





