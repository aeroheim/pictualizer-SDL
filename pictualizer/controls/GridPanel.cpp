#include "GridPanel.h"

GridPanel::GridPanel(int x, int y, int w, int h, int r, int c) : PControl(x, y, w, h)
{
	assert(r > 0);

	int rowHeight = (int) std::round((float) h / r);
	int rowY;

	for (int i = 0; i < r; i++)
	{
		rowY = i == 0 ? y : rowY + rows[i - 1].getHeight();

		GridPanelRow r(x, rowY, w, rowHeight, c);
		rows.push_back(r);
	}
}

void GridPanel::setX(int x)
{
	this->x = x;

	for (GridPanelRow& r : rows)
		r.setX(x);
}

void GridPanel::setY(int y)
{
	this->y = y;

	for (GridPanelRow& r : rows)
		r.setY(y);
}

void GridPanel::setWidth(int width)
{
	assert(width >= 0);

	w = width;

	for (GridPanelRow& r : rows)
		r.setWidth(width);
}

void GridPanel::setColWidths(const std::vector<int>& colWidths)
{
	assert(colWidths.size() == rows[0].getNumCols());

	w = 0;

	for (int i : colWidths)
		w += i;

	for (GridPanelRow& r : rows)
		r.setCellWidths(colWidths);
}

void GridPanel::setHeight(int height)
{
	assert(height >= 0);

	int rowY;

	for (size_t i = 0; i < rows.size(); i++)
	{
		rowY = i == 0 ? y : rowY + rows[i - 1].getHeight();

		rows[i].setY(rowY);

		// Maintain row proportions when we resize their heights.
		rows[i].setHeight((int) std::round(((float) rows[i].getHeight() / h) * height));
	}

	h = height;
}

void GridPanel::setRowHeights(const std::vector<int>& rowHeights)
{
	assert(rowHeights.size() == rows.size());

	h = 0;
	int rowY;

	for (size_t i = 0; i < rowHeights.size(); i++)
	{
		rowY = i == 0 ? y : rowY + rowHeights[i - 1];

		rows[i].setY(rowY);
		rows[i].setHeight(rowHeights[i]);

		h += rowHeights[i];
	}
}

int GridPanel::getColWidth(int c)
{
	assert(c >= 0);
	assert(c < rows[0].getNumCols());

	return rows[0][c].getWidth();
}

const std::vector<int> GridPanel::getColWidths()
{
	std::vector<int> colWidths;

	for (int i = 0; i < rows[0].getNumCols(); i++)
		colWidths.push_back(rows[0][i].getWidth());

	return colWidths;
}

int GridPanel::getRowHeight(int r)
{
	assert(r >= 0);
	assert((size_t) r < rows.size());

	return rows[r].getHeight();
}

const std::vector<int> GridPanel::getRowHeights()
{
	std::vector<int> rowHeights;

	for (GridPanelRow& r : rows)
		rowHeights.push_back(r.getHeight());

	return rowHeights;
}

int GridPanel::getNumRows()
{
	return rows.size();
}

void GridPanel::draw(SDL_Renderer* ren)
{
	for (GridPanelRow& r : rows)
		r.draw(ren);
}

GridPanelRow& GridPanel::operator[](const int index)
{
	assert(index >= 0);
	assert((size_t) index < rows.size());

	return rows[index];
}

void GridPanel::handleEvent(Event* e)
{
	if (!e->handled)
	{

	}
}