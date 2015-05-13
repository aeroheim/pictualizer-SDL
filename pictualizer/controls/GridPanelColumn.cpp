#include "GridPanelColumn.h"

GridPanelColumn::GridPanelColumn(std::vector<GridPanelCell*>& col) : 
	view(*this),
	style(GridPanelColumnStyle::FREE)
{
	for (GridPanelCell* c : col)
		cells.push_back(c);
}

GridPanelColumn::GridPanelColumn(const GridPanelColumn& other) :
	view(*this),
	style(other.style),
	cells(other.cells)
{}

GridPanelColumnView& GridPanelColumn::getView()
{
	return view;
}

void GridPanelColumn::setStyle(GridPanelColumnStyle s)
{
	style = s;
}

GridPanelColumnStyle GridPanelColumn::getStyle() const
{
	return style;
}

void GridPanelColumn::setX(float x)
{
	for (GridPanelCell* c : cells)
		c->setX(x);
}

void GridPanelColumn::setWidth(float w)
{
	if (canResizeWidth(w))
		for (GridPanelCell* c : cells)
			c->setWidth(w);
}

void GridPanelColumn::setMinWidth(float mw)
{
	assert(mw >= 0);

	for (GridPanelCell* c : cells)
		c->setMinWidth(mw);
}

void GridPanelColumn::setMaxWidth(float mw)
{
	assert(mw >= 0);

	for (GridPanelCell* c : cells)
		c->setMaxWidth(mw);
}

bool GridPanelColumn::canResizeWidth(float w) const
{
	for (GridPanelCell* c : cells)
		if (!c->canResizeWidth(w))
			return false;

	return true;
}

float GridPanelColumn::getX() const
{
	assert(cells.size() > 0);
	return cells[0]->getX();
}

float GridPanelColumn::getY() const
{
	assert(cells.size() > 0);
	return cells[0]->getY();
}

float GridPanelColumn::getWidth() const
{
	assert(cells.size() > 0);
	return cells[0]->getWidth();
}

float GridPanelColumn::getHeight() const
{
	assert(cells.size() > 0);
	
	float height = 0; 

	for (GridPanelCell* c : cells)
		height += c->getHeight();

	return height;
}

float GridPanelColumn::getMinWidth() const
{
	assert(cells.size() > 0);
	return cells[0]->getMinWidth();
}

float GridPanelColumn::getMaxWidth() const
{
	assert(cells.size() > 0);
	return cells[0]->getMaxWidth();
}

GridPanelCellView& GridPanelColumn::operator[](const int index)
{
	assert(index >= 0 && (size_t)index < cells.size());

	return cells[index]->getView();
}