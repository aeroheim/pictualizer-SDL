#pragma once

#include <vector>
#include "GridPanelCellView.h"

class GridPanelRow;

/*
 *	Simple wrapper class for GridPanelRow that allows for read-only access.
 */
class GridPanelRowView
{
	public:
		GridPanelRowView(GridPanelRow& row);
		~GridPanelRowView();

		void setStyle(GridPanelRowStyle s);
		GridPanelRowStyle getStyle() const;

		void setMinHeight(float mh);
		void setMaxHeight(float mh);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		float getMinHeight() const;
		float getMaxHeight() const;
		
		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		int getRoundedMinHeight() const;
		int getRoundedMaxHeight() const;

		GridPanelCellView& operator[] (const int index);

	private:
		GridPanelRow& row;
};