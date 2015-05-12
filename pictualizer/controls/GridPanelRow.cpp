#include "GridPanelRow.h"

GridPanelRow::GridPanelRow(std::vector<GridPanelCell>& row) : 
	view(*this),
	style(GridPanelRowStyle::FREE)
{
	for (GridPanelCell& c : row)
		cells.push_back(&c);
}

GridPanelRow::GridPanelRow(const GridPanelRow& other) : 
	view(*this),
	style(other.style),
	cells(other.cells)
{}

GridPanelRowView& GridPanelRow::getView()
{
	return view;
}

void GridPanelRow::setStyle(GridPanelRowStyle s)
{
	style = s;
}

GridPanelRowStyle GridPanelRow::getStyle() const
{
	return style;
}

void GridPanelRow::setY(float y)
{
	for (GridPanelCell* c : cells)
		c->setY(y);
}

void GridPanelRow::setHeight(float h)
{
	assert(h >= 0);

	for (GridPanelCell* c : cells)
		c->setHeight(h);
}

void GridPanelRow::setMinHeight(float mh)
{
	assert(mh >= 0);

	for (GridPanelCell* c : cells)
		c->setMinHeight(mh);
}

void GridPanelRow::setMaxHeight(float mh)
{
	assert(mh >= 0);

	for (GridPanelCell* c : cells)
		c->setMaxHeight(mh);
}

float GridPanelRow::getX() const
{
	assert(cells.size() > 0);
	return cells[0]->getX();
}

float GridPanelRow::getY() const
{
	assert(cells.size() > 0);
	return cells[0]->getY();
}

float GridPanelRow::getWidth() const
{
	assert(cells.size() > 0);

	float width = 0;

	for (GridPanelCell* c : cells)
		width += c->getWidth();

	return width;
}

float GridPanelRow::getHeight() const
{
	assert(cells.size() > 0);
	return cells[0]->getHeight();
}

float GridPanelRow::getMinHeight() const
{
	assert(cells.size() > 0);
	return cells[0]->getMinHeight();
}

float GridPanelRow::getMaxHeight() const
{
	assert(cells.size() > 0);
	return cells[0]->getMaxHeight();
}

GridPanelCellView& GridPanelRow::operator[](const int index)
{
	assert(index >= 0 && (size_t) index < cells.size());

	return cells[index]->getView();
}