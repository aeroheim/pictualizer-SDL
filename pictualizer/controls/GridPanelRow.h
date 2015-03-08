#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelCell.h"

class GridPanelRow : public PControl
{
	public:
		GridPanelRow(int x, int y, int w, int h, int c);
		GridPanelRow(int x, int y, const std::vector<int>& colWidths, int h);

		void setX(int x);
		void setY(int y);
		void setWidth(int width);
		void setCellWidths(const std::vector<int>& cellWidths);
		void setHeight(int height);

		int getNumCols();

		void draw(SDL_Renderer* ren);

		GridPanelCell& operator[] (const int index);

	private:
		std::vector<GridPanelCell> cells;
};