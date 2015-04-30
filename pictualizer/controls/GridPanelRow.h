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
		void setBaseColor(float r, float g, float b);
		void setFocusColor(float r, float g, float b);
		void setColorState(PControlColorState s);
		void setColorStyle(PControlColorStyle s);
		void setColorSpeed(float seconds);

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeStyle(PControlFadeStyle s);
		void setFadeSpeed(float speed);

		void draw(SDL_Renderer* ren);

		GridPanelCell& operator[] (const int index);

	private:
		std::vector<GridPanelCell> cells;
};