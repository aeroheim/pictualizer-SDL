#pragma once

#include "PControl.h"

class GridPanelCell : public PControl
{
	public:
		GridPanelCell(int x, int y, int w, int h);
		GridPanelCell(PControl* e, int x, int y, int w, int h);

		void setElement(PControl* e);
		PControl* getElement();

		void setPadding(int l, int u, int r, int d);

		void setX(int x);
		void setY(int y);
		void setWidth(int width);
		void setHeight(int height);

		void draw();

	private:
		PControl* element;
		int padding[4];
};