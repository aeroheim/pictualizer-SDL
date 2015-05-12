#pragma once

#include <vector>
#include "GridPanelCellView.h"

class GridPanelColumn;

/*
 *	Simple wrapper class for GridPanelRow that allows for read-only access.
 */
class GridPanelColumnView
{
	public:
		GridPanelColumnView(GridPanelColumn& col);
		~GridPanelColumnView();

		void setStyle(GridPanelColumnStyle s);
		GridPanelColumnStyle getStyle() const;

		void setMinWidth(float mw);
		void setMaxWidth(float mw);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		float getMinWidth() const;
		float getMaxWidth() const;

		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		int getRoundedMinWidth() const;
		int getRoundedMaxWidth() const;

		GridPanelCellView& operator[] (const int index);

	private:
		GridPanelColumn& col;
};