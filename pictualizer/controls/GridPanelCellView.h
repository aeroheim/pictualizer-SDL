#pragma once

#include <vector>
#include "PControl.h"

class GridPanelCell;

/*
 *	Simple wrapper class for GridPanelCell that allows for read-only access and setting of elements.
 */
class GridPanelCellView
{
	public:
		GridPanelCellView(GridPanelCell& cell);
		~GridPanelCellView();

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		void setElement(PControl* element);
		PControl* getElement() const;

		void setPadding(float l, float u, float r, float d);
		std::vector<float> getPadding() const;

	private:
		GridPanelCell& cell;
};