#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelRow.h"
#include "../io/EventSubscriber.h"
#include "../config/PGlobals.h"

class GridPanel : public PControl, public EventSubscriber
{
	public:
		GridPanel(float x, float y, float w, float h, int r, int c);
		GridPanel(float x, float y, const std::vector<float>& rowHeights, const std::vector<float>& colWidths);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setRowMinHeight(int r, float mh);
		void setRowMaxHeight(int r, float mh);
		void setRowHeights(const std::vector<float>& rowHeights);
		float getRowHeight(int r);
		std::vector<float> getRowHeights() const;
		int getNumRows() const;

		void setColMinWidth(int c, float mw);
		void setColMaxWidth(int c, float mw);
		void setColWidths(const std::vector<float>& colWidths);
		float getColWidth(int c);
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

		GridPanelRowView& operator[] (const int index);

		void handleEvent(Event* e);

	private:
		std::vector<GridPanelRow> rows;
};