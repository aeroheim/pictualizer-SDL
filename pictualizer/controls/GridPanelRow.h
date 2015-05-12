#pragma once

#pragma once

#include <vector>
#include <numeric>
#include <cassert>
#include "GridPanelCell.h"
#include "GridPanelRowView.h"
#include "GridPanelStates.h"

class GridPanelRow
{
	public:
		GridPanelRow(std::vector<GridPanelCell>& row);
		GridPanelRow(const GridPanelRow& other);

		GridPanelRowView& getView();

		void setStyle(GridPanelRowStyle s);
		GridPanelRowStyle getStyle() const;

		void setY(float y);
		void setHeight(float h);

		void setMinHeight(float mh);
		void setMaxHeight(float mh);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		float getMinHeight() const;
		float getMaxHeight() const;

		GridPanelCellView& operator[] (const int index);

	private:
		GridPanelRowView view;
		GridPanelRowStyle style;
		std::vector<GridPanelCell*> cells;
};