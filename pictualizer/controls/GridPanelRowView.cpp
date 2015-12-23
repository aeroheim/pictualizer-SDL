#include "GridPanelRow.h"
#include "GridPanelRowView.h"

GridPanelRowView::GridPanelRowView(GridPanelRow& row) : row(row) {}

GridPanelRowView::~GridPanelRowView() {}

float GridPanelRowView::getX() const
{
	return row.getX();
}

float GridPanelRowView::getY() const
{
	return row.getY();
}

float GridPanelRowView::getWidth() const
{
	return row.getWidth();
}

float GridPanelRowView::getHeight() const
{
	return row.getHeight();
}

int GridPanelRowView::getRoundedX() const
{
	return row.getRoundedX();
}

int GridPanelRowView::getRoundedY() const
{
	return row.getRoundedY();
}

int GridPanelRowView::getRoundedWidth() const
{
	return row.getRoundedWidth();
}

int GridPanelRowView::getRoundedHeight() const
{
	return row.getRoundedHeight();
}

int GridPanelRowView::getNumCols() const
{
	return row.getNumCols();
}

std::vector<float> GridPanelRowView::getColWidths() const
{
	return row.getColWidths();
}

GridPanelCellView& GridPanelRowView::operator[] (const int index)
{
	return row[index].getView();
}

