#include "GridPanelCell.h"

GridPanelCell::GridPanelCell(int x, int y, int w, int h) : PControl(x, y, w, h)
{
	element = nullptr;
}

GridPanelCell::GridPanelCell(PControl* e, int x, int y, int w, int h) : GridPanelCell(x, y, w, h)
{
	e->setX(x);
	e->setY(y);
	e->setWidth(w);
	e->setHeight(h);
}

void GridPanelCell::setElement(PControl* e)
{
	element = e;
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
	element->setWidth(width - padding[2] - padding[0]);
	this->w = width;
}

void GridPanelCell::setHeight(int height)
{
	element->setHeight(height - padding[3] - padding[1]);
	this->h = height;
}

void GridPanelCell::draw()
{
	element->draw();
}





