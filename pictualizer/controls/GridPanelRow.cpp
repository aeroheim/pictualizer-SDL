#include "GridPanelRow.h"

GridPanelRow::GridPanelRow(int x, int y, int w, int h, int c) : PControl(x, y, w, h)
{
	assert(c > 0);

	int cellWidth = (int) std::round((float) w / c);

	for (int i = 0; i < c; i++)
	{
		GridPanelCell c(x + i * cellWidth, y, cellWidth, h);
		cells.push_back(c);
	}
}

GridPanelRow::GridPanelRow(int x, int y, const std::vector<int>& colWidths, int h) : PControl(x, y, std::accumulate(colWidths.begin(), colWidths.end(), 0), h)
{
	assert(colWidths.size() > 0);

	int cellX;

	for (size_t i = 0; i < colWidths.size(); i++)
	{
		cellX = i == 0 ? x : cellX + colWidths[i - 1];

		GridPanelCell c(cellX, y, colWidths[i], h);
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

	int cellX;

	for (size_t i = 0; i < cells.size(); i++)
	{
		cellX = i == 0 ? x : cellX + cells[i - 1].getWidth();

		cells[i].setX(cellX);

		// Maintain cell proportions when we resize their widths;
		cells[i].setWidth((int) std::round(((float) cells[i].getWidth() / w) * width));
	}

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