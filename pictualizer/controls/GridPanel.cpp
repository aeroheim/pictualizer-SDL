#include "GridPanel.h"

GridPanel::GridPanel(int x, int y, int w, int h, int r, int c) : PControl(x, y, w, h)
{
	float rowHeight = (float) h / r;
	float colWidth = (float) w / c;

	for (int i = 0; i < r; i++)
	{
		rowHeights.push_back(rowHeight);
		controls.push_back(std::vector<PControl*>(c, nullptr));
	}

	for (int i = 0; i < c; i++)
		colWidths.push_back(colWidth);
}

void GridPanel::setRowHeights(const std::vector<int>& heights)
{
	if (rowHeights.size() == heights.size())
		rowHeights = heights;
	else
		throw "GridPanel::setRowHeights(): size mismatch.";
}

void GridPanel::setColWidths(const std::vector<int>& widths)
{
	if (colWidths.size() == widths.size())
		colWidths = widths;
	else
		throw "GridPanel::setColWidths(): size mismatch.";
}

int GridPanel::getRowHeight(int r)
{
	if (r >= 0 && (size_t) r < rowHeights.size())
		return rowHeights[r];

	return -1;
}

int GridPanel::getColWidth(int c)
{
	if (c >= 0 && (size_t) c < colWidths.size())
		return colWidths[c];

	return -1;
}

const std::vector<int>& GridPanel::getRowHeights()
{
	return rowHeights;
}

const std::vector<int>& GridPanel::getColWidths()
{
	return colWidths;
}

void GridPanel::setControl(PControl* control, int r, int c)
{
	if ((r >= 0 && (size_t)r < controls.size()) && (c >= 0 && (size_t)c < controls[r].size()))
		controls[r][c] = control;
	else
		throw "GridPanel::setControl(): invalid index.";
}

void GridPanel::draw()
{
	for (auto r : controls)
		for (PControl* control : r)
			control->draw();
}