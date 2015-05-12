#include "GridPanelRow.h"
#include "GridPanelRowView.h"

GridPanelRowView::GridPanelRowView(GridPanelRow& row) : row(row) {}

GridPanelRowView::~GridPanelRowView() {}

void GridPanelRowView::setStyle(GridPanelRowStyle s)
{
	row.setStyle(s);
}

GridPanelRowStyle GridPanelRowView::getStyle() const
{
	return row.getStyle();
}

void GridPanelRowView::setMinHeight(float mh)
{
	row.setMinHeight(mh);
}

void GridPanelRowView::setMaxHeight(float mh)
{
	row.setMaxHeight(mh);
}

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

float GridPanelRowView::getMinHeight() const
{
	return row.getMinHeight();
}

float GridPanelRowView::getMaxHeight() const
{
	return row.getMaxHeight();
}

int GridPanelRowView::getRoundedX() const
{
	return (int) std::round(row.getX());
}

int GridPanelRowView::getRoundedY() const
{
	return (int) std::round(row.getY);
}

int GridPanelRowView::getRoundedWidth() const
{
	return (int) std::round(row.getWidth());
}

int GridPanelRowView::getRoundedHeight() const
{
	return (int) std::round(row.getHeight());
}

int GridPanelRowView::getRoundedMinHeight() const
{
	return (int) std::round(row.getMinHeight());
}

int GridPanelRowView::getRoundedMaxHeight() const
{
	return (int) std::round(row.getMaxHeight());
}

GridPanelCellView& GridPanelRowView::operator[] (const int index)
{
	return row[index];
}

