#pragma once

#include <vector>
#include "GridPanelCell.h"
#include "GridPanelRowView.h"

class GridPanelRow : public PControl
{
	public:
		GridPanelRow(float x, float y, float w, float h, int c);
		GridPanelRow(float x, float y, const std::vector<float>& colWidths, float h);

		GridPanelRow(const GridPanelRow& other);

		GridPanelRowView& getView();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColWidths(const std::vector<float>& cellWidths);
		std::vector<float> getColWidths() const;
		int getNumCols() const;

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
		GridPanelRowView view;
		std::vector<GridPanelCell> cells;
};