#pragma once

#include "GridPanelCell.h"
#include "GridPanelCellView.h"

GridPanelCellView::GridPanelCellView(GridPanelCell& cell) : cell(cell) {}

GridPanelCellView::~GridPanelCellView() {}

float GridPanelCellView::getX() const
{
	return cell.getX();
}

float GridPanelCellView::getY() const
{
	return cell.getY();
}

float GridPanelCellView::getWidth() const
{
	return cell.getWidth();
}

float GridPanelCellView::getHeight() const
{
	return cell.getHeight();
}

int GridPanelCellView::getRoundedX() const
{
	return cell.getRoundedX();
}

int GridPanelCellView::getRoundedY() const
{
	return cell.getRoundedY();
}

int GridPanelCellView::getRoundedWidth() const
{
	return cell.getRoundedWidth();
}

int GridPanelCellView::getRoundedHeight() const
{
	return cell.getRoundedHeight();
}

void GridPanelCellView::setElement(PControl* element)
{
	cell.setElement(element);
}

PControl* GridPanelCellView::getElement() const
{
	return cell.getElement();
}

void GridPanelCellView::setPadding(float l, float u, float r, float d)
{
	cell.setPadding(l, u, r, d);
}

std::vector<float> GridPanelCellView::getPadding() const
{
	return cell.getPadding();
}