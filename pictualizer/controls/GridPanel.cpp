#include "GridPanel.h"

GridPanel::GridPanel(float x, float y, float w, float h, int r, int c) : PControl(x, y, w, h)
{
	assert(r > 0);

	float rowHeight = h / r;

	for (int i = 0; i < r; i++)
	{
		GridPanelRow r(x, y + (rowHeight * i), w, rowHeight, c);
		rows.push_back(r);
	}
}

GridPanel::GridPanel(float x, float y, const std::vector<float>& rowHeights, const std::vector<float>& colWidths) : PControl(x, y, 
																															std::accumulate(colWidths.begin(), colWidths.end(), 0.0f),
																															std::accumulate(rowHeights.begin(), rowHeights.end(), 0.0f))
{
	assert(rowHeights.size() > 0);

	float rowY;

	for (size_t i = 0; i < rowHeights.size(); i++)
	{
		rowY = i == 0 ? y : rowY + rowHeights[i - 1];

		GridPanelRow r(x, rowY, colWidths, rowHeights[i]);
		rows.push_back(r);
	}
}

void GridPanel::setX(float x)
{
	PControl::setX(x);

	for (GridPanelRow& r : rows)
		r.setX(x);
}

void GridPanel::setY(float y)
{
	PControl::setY(y);

	for (size_t i = 0; i < rows.size(); i++)
	{
		if (i == 0)
			rows[i].setY(y);
		else
			rows[i].setY(rows[i - 1].getY() + rows[i - 1].getHeight());
	}
}

void GridPanel::setWidth(float w)
{
	assert(w >= 0);

	PControl::setWidth(w);

	for (GridPanelRow& r : rows)
		r.setWidth(w);
}

void GridPanel::setColWidths(const std::vector<float>& colWidths)
{
	assert(colWidths.size() == rows[0].getNumCols());

	PControl::setWidth(std::accumulate(colWidths.begin(), colWidths.end(), 0.0f));

	for (GridPanelRow& r : rows)
		r.setCellWidths(colWidths);
}

void GridPanel::setHeight(float h)
{
	assert(h >= 0);

	float rowY;

	for (size_t i = 0; i < rows.size(); i++)
	{
		rowY = i == 0 ? getY() : rowY + rows[i - 1].getHeight();

		rows[i].setY(rowY);

		// Maintain row proportions when we resize their heights.
		rows[i].setHeight((rows[i].getHeight() / getHeight()) * h);
	}

	PControl::setHeight(h);
}

void GridPanel::setRowHeights(const std::vector<float>& rowHeights)
{
	assert(rowHeights.size() == rows.size());

	float h = 0;
	float rowY;

	for (size_t i = 0; i < rowHeights.size(); i++)
	{
		rowY = i == 0 ? getY() : rowY + rowHeights[i - 1];

		rows[i].setY(rowY);
		rows[i].setHeight(rowHeights[i]);

		h += rowHeights[i];
	}

	PControl::setHeight(h);
}

float GridPanel::getColWidth(int c)
{
	assert(c >= 0);
	assert(c < rows[0].getNumCols());

	return rows[0][c].getWidth();
}

const std::vector<float> GridPanel::getColWidths()
{
	std::vector<float> colWidths;

	for (int i = 0; i < rows[0].getNumCols(); i++)
		colWidths.push_back(rows[0][i].getWidth());

	return colWidths;
}

float GridPanel::getRowHeight(int r)
{
	assert(r >= 0);
	assert((size_t) r < rows.size());

	return rows[r].getHeight();
}

const std::vector<float> GridPanel::getRowHeights()
{
	std::vector<float> rowHeights;

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