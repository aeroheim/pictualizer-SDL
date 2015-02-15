#include "GridPanelCell.h"

GridPanelCell::GridPanelCell(int x, int y, int w, int h) : PControl(x, y, w, h)
{
	element = nullptr;

	for (int i = 0; i < 4; i++)
		padding[i] = 0;
}

GridPanelCell::GridPanelCell(PControl* e, int x, int y, int w, int h) : PControl(x, y, w, h)
{
	e->setX(x);
	e->setY(y);
	e->setWidth(w);
	e->setHeight(h);
}

void GridPanelCell::setElement(PControl* e)
{
	element = e;
	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
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

	setX(x);
	setY(y);
	setWidth(w);
	setHeight(h);
}

void GridPanelCell::setX(int x)
{
	element->setX(x + padding[0]);
	this->x = x;
}

void GridPanelCell::setY(int y)
{
	element->setY(y + padding[1]);
	this->y = y;
}

void GridPanelCell::setWidth(int width)
{
	assert(width >= 0);

	element->setWidth(width - padding[2] - padding[0]);
	this->w = width;
}

void GridPanelCell::setHeight(int height)
{
	assert(height >= 0);

	element->setHeight(height - padding[3] - padding[1]);
	this->h = height;
}

void GridPanelCell::draw(SDL_Renderer* ren)
{
	if (element)
		element->draw(ren);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderDrawLine(ren, x, y, x + w, y);
	SDL_RenderDrawLine(ren, x, y, x, y + h);
	SDL_RenderDrawLine(ren, x + w, y, x + w, y + h);
	SDL_RenderDrawLine(ren, x, y + h, x + w, y + h);

	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderDrawPoint(ren, x, y);
	SDL_RenderDrawPoint(ren, x + w, y);
	SDL_RenderDrawPoint(ren, x, y + h);
	SDL_RenderDrawPoint(ren, x + w, y + h);
}





