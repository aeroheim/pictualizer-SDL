#pragma once

#include <vector>
#include "PControl.h"

class GridPanel : public PControl
{
	public:
		GridPanel(int x, int y, int w, int h, int r, int c);

		void setRowHeights(const std::vector<int>& heights);
		void setColWidths(const std::vector<int>& widths);
		int getRowHeight(int r);
		int getColWidth(int c);
		const std::vector<int>& getRowHeights();
		const std::vector<int>& getColWidths();

		void setControl(PControl* control, int r, int c);

		virtual void draw();

	private:
		std::vector<std::vector<PControl*>> controls;
		std::vector<int> rowHeights;
		std::vector<int> colWidths;
};