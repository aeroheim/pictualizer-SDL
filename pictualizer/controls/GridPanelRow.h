#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelCell.h"

class GridPanelRow : public PControl
{
	public:
		GridPanelRow(float x, float y, float w, float h, int c);
		GridPanelRow(float x, float y, const std::vector<float>& colWidths, float h);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setCellWidths(const std::vector<float>& cellWidths);

		int getNumCols();

		void setColor(float r, float g, float b);
		void setAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeDelta(float delta);

		void draw(SDL_Renderer* ren);

		GridPanelCell& operator[] (const int index);

	private:
		std::vector<GridPanelCell> cells;
};