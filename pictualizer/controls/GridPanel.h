#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelRow.h"
#include "../io/EventSubscriber.h"

class GridPanel : public PControl, public EventSubscriber
{
	public:
		GridPanel(float x, float y, float w, float h, int r, int c);
		GridPanel(float x, float y, const std::vector<float>& rowHeights, const std::vector<float>& colWidths);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setColWidths(const std::vector<float>& colWidths);
		void setHeight(float h);
		void setRowHeights(const std::vector<float>& rowHeights);

		float getColWidth(int c);
		const std::vector<float> getColWidths();
		float getRowHeight(int r);
		const std::vector<float> getRowHeights();

		int getNumRows();

		void draw(SDL_Renderer* ren);

		GridPanelRow& operator[] (const int index);

		void handleEvent(Event* e);

	private:
		std::vector<GridPanelRow> rows;
};