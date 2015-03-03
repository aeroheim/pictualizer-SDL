#pragma once

#include <vector>
#include <cassert>
#include "GridPanelRow.h"
#include "../io/EventSubscriber.h"

class GridPanel : public PControl, public EventSubscriber
{
	public:
		GridPanel(int x, int y, int w, int h, int r, int c);

		void setX(int x);
		void setY(int y);
		void setWidth(int width);
		void setColWidths(const std::vector<int>& colWidths);
		void setHeight(int height);
		void setRowHeights(const std::vector<int>& rowHeights);

		int getColWidth(int c);
		const std::vector<int> getColWidths();
		int getRowHeight(int r);
		const std::vector<int> getRowHeights();

		void draw(SDL_Renderer* ren);

		GridPanelRow& operator[] (const int index);

		void handleEvent(Event* e);

	private:
		std::vector<GridPanelRow> rows;
};