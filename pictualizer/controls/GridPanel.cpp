#include "GridPanel.h"

using namespace PGlobals;

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

GridPanel::GridPanel(float x, float y, const std::vector<float>& rowHeights, const std::vector<float>& colWidths) : 
	PControl(x, y, std::accumulate(colWidths.begin(), colWidths.end(), 0.0f), std::accumulate(rowHeights.begin(), rowHeights.end(), 0.0f))
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

void GridPanel::setRowMinHeight(int r, float mh)
{
	assert(r >= 0 && (size_t)r < rows.size());

	for (GridPanelRow& r : rows)
		r.setMinHeight(mh);
}

void GridPanel::setRowMaxHeight(int r, float mh)
{
	assert(r >= 0 && (size_t)r < rows.size());

	for (GridPanelRow& r : rows)
		r.setMaxHeight(mh);
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

float GridPanel::getRowHeight(int r)
{
	assert(r >= 0 && (size_t)r < rows.size());

	return rows[r].getHeight();
}

std::vector<float> GridPanel::getRowHeights() const
{
	std::vector<float> rowHeights;

	for (const GridPanelRow& r : rows)
		rowHeights.push_back(r.getHeight());

	return rowHeights;
}

int GridPanel::getNumRows() const
{
	return rows.size();
}

void GridPanel::setColMinWidth(int c, float mw)
{
	assert(c >= 0 && c < rows[0].getNumCols());

	for (GridPanelRow& r : rows)
		r[c].setMinWidth(mw);
}

void GridPanel::setColMaxWidth(int c, float mw)
{
	assert(c >= 0 && c < rows[0].getNumCols());

	for (GridPanelRow& r : rows)
		r[c].setMaxWidth(mw);
}

void GridPanel::setColWidths(const std::vector<float>& colWidths)
{
	assert(colWidths.size() == rows[0].getNumCols());

	PControl::setWidth(std::accumulate(colWidths.begin(), colWidths.end(), 0.0f));

	for (GridPanelRow& r : rows)
		r.setColWidths(colWidths);
}

float GridPanel::getColWidth(int c)
{
	assert(c >= 0 && c < rows[0].getNumCols());

	return rows[0][c].getWidth();
}

std::vector<float> GridPanel::getColWidths() const
{
	return rows[0].getColWidths();
}

int GridPanel::getNumCols() const
{
	return rows[0].getNumCols();
}

void GridPanel::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	for (GridPanelRow& row : rows)
		row.setColor(r, g, b);
}

void GridPanel::setBaseColor(float r, float g, float b)
{
	PControl::setBaseColor(r, g, b);

	for (GridPanelRow& row : rows)
		row.setBaseColor(r, g, b);
}

void GridPanel::setFocusColor(float r, float g, float b)
{
	PControl::setFocusColor(r, g, b);

	for (GridPanelRow& row : rows)
		row.setFocusColor(r, g, b);
}

void GridPanel::setColorState(PControlColorState s)
{
	PControl::setColorState(s);

	for (GridPanelRow& r : rows)
		r.setColorState(s);
}

void GridPanel::setColorStyle(PControlColorStyle s)
{
	PControl::setColorStyle(s);

	for (GridPanelRow& r : rows)
		r.setColorStyle(s);
}

void GridPanel::setColorSpeed(float seconds)
{
	PControl::setColorSpeed(seconds);

	for (GridPanelRow& r : rows)
		r.setColorSpeed(seconds);
}

void GridPanel::setAlpha(float a)
{
	PControl::setAlpha(a);

	for (GridPanelRow& r : rows)
		r.setAlpha(a);
}

void GridPanel::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);

	for (GridPanelRow& r : rows)
		r.setMinAlpha(a);
}

void GridPanel::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);

	for (GridPanelRow& r : rows)
		r.setMaxAlpha(a);
}

void GridPanel::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);

	for (GridPanelRow& r : rows)
		r.setFadeState(s);
}

void GridPanel::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);

	for (GridPanelRow& r : rows)
		r.setFadeStyle(s);
}

void GridPanel::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);

	for (GridPanelRow& r : rows)
		r.setFadeSpeed(seconds);
}

void GridPanel::draw(SDL_Renderer* ren)
{
	for (GridPanelRow& row : rows)
		row.draw(ren);

	PControl::draw(ren);
}

GridPanelRowView& GridPanel::operator[](const int index)
{
	assert(index >= 0 && (size_t) index < rows.size());

	return rows[index].getView();
}

void GridPanel::handleEvent(Event* e)
{
	if (!e->handled)
	{

	}
}