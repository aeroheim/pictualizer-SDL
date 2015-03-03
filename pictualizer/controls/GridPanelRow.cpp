#include "GridPanelRow.h"

GridPanelRow::GridPanelRow(int x, int y, int w, int h, int c) : PControl(x, y, w, h)
{
	assert(c > 0);
	assert(w >= c);

	int cellWidth = (int) std::floor((float) w / c);

	for (int i = 0; i < c; i++)
	{
		GridPanelCell c(x + i * cellWidth, y, cellWidth, h);
		cells.push_back(c);
	}
}

void GridPanelRow::setX(int x)
{
	this->x = x;

	for (size_t i = 0; i < cells.size(); i++)
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

	for (GridPanelCell& c : cells)
		c.setY(y);
}

void GridPanelRow::setWidth(int width)
{
	assert(width >= 0);

	// Maintain cell proportions when we resize their widths;
	for (GridPanelCell& c : cells)
		c.setWidth((int) std::floor(((float) c.getWidth() / w) * width));

	w = width;
}

void GridPanelRow::setCellWidths(const std::vector<int>& cellWidths)
{
	assert(cellWidths.size() == cells.size());

	w = 0;
	int cellX;

	for (size_t i = 0; i < cellWidths.size(); i++)
	{
		cellX = i == 0 ? x : cellX + cellWidths[i - 1];

		cells[i].setX(cellX);
		cells[i].setWidth(cellWidths[i]);

		w += cellWidths[i];
	}
}

void GridPanelRow::setHeight(int height)
{
	assert(height >= 0);

	h = height;

	for (GridPanelCell& c : cells)
		c.setHeight(height);
}

int GridPanelRow::getNumCols()
{
	return cells.size();
}

void GridPanelRow::draw(SDL_Renderer* ren)
{
	for (GridPanelCell& c : cells)
		c.draw(ren);
}

GridPanelCell& GridPanelRow::operator[](const int index)
{
	assert(index >= 0);
	assert((size_t) index < cells.size());

	return cells[index];
}