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

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;
		
		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		int getNumCols() const;
		std::vector<float> getColWidths() const;

		GridPanelCellView& operator[] (const int index);

	private:
		GridPanelRow& row;
};