#pragma once

#include "GridPanelRow.h"
#include <numeric>
#include <cassert>

GridPanelRow::GridPanelRow(float x, float y, float w, float h, int c) : 
	PControl(x, y, w, h),
	view(*this)
{
	assert(c > 0);

	float cellWidth = getWidth() / c;

	for (int i = 0; i < c; i++)
	{
		GridPanelCell c(x + i * cellWidth, y, cellWidth, h);
		cells.push_back(c);
	}
}

GridPanelRow::GridPanelRow(float x, float y, const std::vector<float>& colWidths, float h) : 
	PControl(x, y, std::accumulate(colWidths.begin(), colWidths.end(), 0.0f), h),
	view(*this)
{
	assert(colWidths.size() > 0);

	float cellX;

	for (size_t i = 0; i < colWidths.size(); i++)
	{
		cellX = i == 0 ? getX() : cellX + colWidths[i - 1];

		GridPanelCell c(cellX, y, colWidths[i], h);
		cells.push_back(c);
	}
}

GridPanelRow::GridPanelRow(const GridPanelRow& other) : 
	PControl(other), 
	view(*this),
	cells(other.cells)
{}


GridPanelRowView& GridPanelRow::getView()
{
	return view;
}

void GridPanelRow::setX(float x)
{
	PControl::setX(x);

	for (size_t i = 0; i < cells.size(); i++)
	{
		if (i == 0)
			cells[i].setX(x);
		else
			cells[i].setX(cells[i - 1].getX() + cells[i - 1].getWidth());
	}
}

void GridPanelRow::setY(float y)
{
	PControl::setY(y);

	for (GridPanelCell& c : cells)
		c.setY(y);
}

void GridPanelRow::setWidth(float w)
{
	assert(w >= 0);

	float cellX;

	for (size_t i = 0; i < cells.size(); i++)
	{
		cellX = i == 0 ? getX() : cellX + cells[i - 1].getWidth();

		cells[i].setX(cellX);

		// Maintain cell proportions when we resize their widths;
		cells[i].setWidth((cells[i].getWidth() / getWidth()) * w);
	}

	PControl::setWidth(w);
}

void GridPanelRow::setHeight(float h)
{
	assert(h >= 0);

	PControl::setHeight(h);

	for (GridPanelCell& c : cells)
		c.setHeight(h);
}

void GridPanelRow::setColWidths(const std::vector<float>& cellWidths)
{
	assert(cellWidths.size() == cells.size());

	float w = 0;
	float cellX;

	for (size_t i = 0; i < cellWidths.size(); i++)
	{
		cellX = i == 0 ? getX() : cellX + cellWidths[i - 1];

		cells[i].setX(cellX);
		cells[i].setWidth(cellWidths[i]);

		w += cellWidths[i];
	}

	PControl::setWidth(w);
}

std::vector<float> GridPanelRow::getColWidths() const
{
	std::vector<float> colWidths;

	for (const GridPanelCell& c : cells)
		colWidths.push_back(c.getWidth());

	return colWidths;
}

int GridPanelRow::getNumCols() const
{
	return cells.size();
}

void GridPanelRow::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	for (GridPanelCell& c : cells)
		c.setColor(r, g, b);
}

void GridPanelRow::setBaseColor(float r, float g, float b)
{
	PControl::setBaseColor(r, g, b);

	for (GridPanelCell& c : cells)
		c.setBaseColor(r, g, b);
}

void GridPanelRow::setFocusColor(float r, float g, float b)
{
	PControl::setFocusColor(r, g, b);

	for (GridPanelCell& c : cells)
		c.setFocusColor(r, g, b);
}

void GridPanelRow::setColorState(PControlColorState s)
{
	PControl::setColorState(s);

	for (GridPanelCell& c : cells)
		c.setColorState(s);
}

void GridPanelRow::setColorStyle(PControlColorStyle s)
{
	PControl::setColorStyle(s);

	for (GridPanelCell& c : cells)
		c.setColorStyle(s);
}

void GridPanelRow::setColorSpeed(float seconds)
{
	PControl::setColorSpeed(seconds);

	for (GridPanelCell& c : cells)
		c.setColorSpeed(seconds);
}

void GridPanelRow::setAlpha(float a)
{
	PControl::setAlpha(a);

	for (GridPanelCell& c : cells)
		c.setAlpha(a);
}

void GridPanelRow::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);

	for (GridPanelCell& c : cells)
		c.setMinAlpha(a);
}

void GridPanelRow::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);

	for (GridPanelCell& c : cells)
		c.setMaxAlpha(a);
}

void GridPanelRow::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);

	for (GridPanelCell& c : cells)
		c.setFadeState(s);
}

void GridPanelRow::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);

	for (GridPanelCell& c : cells)
		c.setFadeStyle(s);
}

void GridPanelRow::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);

	for (GridPanelCell& c : cells)
		c.setFadeSpeed(seconds);
}

void GridPanelRow::draw(SDL_Renderer* ren)
{
	for (GridPanelCell& c : cells)
		c.draw(ren);

	PControl::draw(ren);
}

GridPanelCell& GridPanelRow::operator[](const int index)
{
	assert(index >= 0 && (size_t) index < cells.size());

	return cells[index];
}