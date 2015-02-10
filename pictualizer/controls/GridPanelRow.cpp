#include "GridPanelRow.h"

GridPanelRow::GridPanelRow(int x, int y, int w, int h, int c) : PControl(x, y, w, h)
{
	int cellWidth = (int) std::floor((float) w / c);

	for (int i = 0; i < c; i++)
	{
		GridPanelCell c(x, y, cellWidth, h);
		cells.push_back(c);
	}
}

void GridPanelRow::setX(int x)
{
	this->x = x;

	for (int i = 0; i < cells.size(); i++)
	{
		if (i == 0)
			cells[i].setX(x);
		else
			cells[i].setX(cells[i - 1].getX() + cells[i - 1].getWidth());
	}
}

void GridPanelRow::setY(int y)
{
	this->y = y;

	for (GridPanelCell c : cells)
		c.setY(y);
}

void GridPanelRow::setWidth(int width)
{
	// Maintain cell proportions when we resize their widths;
	for (GridPanelCell c : cells)
		c.setWidth((int) std::floor(((float) c.getWidth() / w) * width));

	w = width;
}

void GridPanelRow::setCellWidths(const std::vector<int>& cellWidths)
{
	assert(cellWidths.size() == cells.size());

	w = 0;

	for (int i = 0; i < cellWidths.size(); i++)
	{
		cells[i].setWidth(cellWidths[i]);
		w += cellWidths[i];
	}
}

void GridPanelRow::setHeight(int height)
{
	h = height;

	for (GridPanelCell c : cells)
		c.setHeight(height);
}

int GridPanelRow::getNumCols()
{
	return cells.size();
}

void GridPanelRow::draw()
{
	for (GridPanelCell c : cells)
		c.draw();
}

GridPanelCell& GridPanelRow::operator[](const int index)
{
	return cells[index];
}