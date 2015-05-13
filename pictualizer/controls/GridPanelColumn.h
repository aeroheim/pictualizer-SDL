#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelCell.h"
#include "GridPanelColumnView.h"
#include "GridPanelStates.h"

class GridPanelColumn
{
	public:
		GridPanelColumn(std::vector<GridPanelCell*>& col);
		GridPanelColumn(const GridPanelColumn& other);

		GridPanelColumnView& getView();

		void setStyle(GridPanelColumnStyle s);
		GridPanelColumnStyle getStyle() const;

		void setX(float x);
		void setWidth(float w);

		void setMinWidth(float mw);
		void setMaxWidth(float mw);

		bool canResizeWidth(float w) const;

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		float getMinWidth() const;
		float getMaxWidth() const;

		GridPanelCellView& operator[] (const int index);

	private:
		GridPanelColumnView view;
		GridPanelColumnStyle style;
		std::vector<GridPanelCell*> cells;
};