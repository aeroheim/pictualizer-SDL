#include "GridPanelColumn.h"
#include "GridPanelColumnView.h"

GridPanelColumnView::GridPanelColumnView(GridPanelColumn& col) : col(col) {}

GridPanelColumnView::~GridPanelColumnView() {}

void GridPanelColumnView::setStyle(GridPanelColumnStyle s)
{
	col.setStyle(s);
}

GridPanelColumnStyle GridPanelColumnView::getStyle() const
{
	return col.getStyle();
}

void GridPanelColumnView::setMinWidth(float mw)
{
	col.setMinWidth(mw);
}

void GridPanelColumnView::setMaxWidth(float mh)
{
	col.setMaxWidth(mh);
}

float GridPanelColumnView::getX() const
{
	return col.getX();
}

float GridPanelColumnView::getY() const
{
	return col.getY();
}

float GridPanelColumnView::getWidth() const
{
	return col.getWidth();
}

float GridPanelColumnView::getHeight() const
{
	return col.getHeight();
}

float GridPanelColumnView::getMinWidth() const
{
	return col.getMinWidth();
}

float GridPanelColumnView::getMaxWidth() const
{
	return col.getMaxWidth();
}

int GridPanelColumnView::getRoundedX() const
{
	return (int) std::round(col.getX());
}

int GridPanelColumnView::getRoundedY() const
{
	return (int) std::round(col.getY);
}

int GridPanelColumnView::getRoundedWidth() const
{
	return (int) std::round(col.getWidth());
}

int GridPanelColumnView::getRoundedHeight() const
{
	return (int) std::round(col.getHeight());
}

int GridPanelColumnView::getRoundedMinWidth() const
{
	return (int) std::round(col.getMinWidth());
}

int GridPanelColumnView::getRoundedMaxWidth() const
{
	return (int) std::round(col.getMaxWidth());
}

GridPanelCellView& GridPanelColumnView::operator[] (const int index)
{
	return col[index];
}

