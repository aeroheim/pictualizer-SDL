#include "GridPanel.h"

using namespace PGlobals;

GridPanel::GridPanel(float x, float y, float w, float h, int r, int c) : PControl(x, y, w, h)
{
	assert(r > 0);

	float cellWidth = w / c;
	float cellHeight = h / r;

	for (int i = 0; i < r; i++)
	{
		// Create the GridPanelCells.
		for (int j = 0; j < c; j++)
		{
			GridPanelCell cell(x + j * cellWidth, y + i * cellHeight, cellWidth, cellHeight);
			cells[i].push_back(cell);
		}

		// Create the GridPanelRows.
		GridPanelRow row(cells[i]);
		rows.push_back(row);
	}

	for (int i = 0; i < c; i++)
	{
		std::vector<GridPanelCell*> tempCol;

		for (int j = 0; j < r; j++)
			tempCol.push_back(&cells[j][i]);

		// Create the GridPanelColumns.
		GridPanelColumn col(tempCol);
		cols.push_back(col);
	}
}

GridPanel::GridPanel(float x, float y, const std::vector<float>& rowHeights, const std::vector<float>& colWidths) : 
	PControl(x, y, std::accumulate(colWidths.begin(), colWidths.end(), 0.0f), std::accumulate(rowHeights.begin(), rowHeights.end(), 0.0f))
{
	assert(rowHeights.size() > 0);

	float colX;
	float rowY;

	for (size_t i = 0; i < rowHeights.size(); i++)
	{
		rowY = i == 0 ? y : rowY + rowHeights[i - 1];

		// Create the GridPanelCells.
		for (size_t j = 0; j < colWidths.size(); j++)
		{
			colX = j == 0 ? x : colX + colWidths[j - 1];

			GridPanelCell cell(colX, rowY, colWidths[j], rowHeights[i]);
			cells[i].push_back(cell);
		}

		// Create the GridPanelRows.
		GridPanelRow row(cells[i]);
		rows.push_back(row);
	}

	for (size_t i = 0; i < colWidths.size(); i++)
	{
		std::vector<GridPanelCell*> tempCol;

		for (size_t j = 0; j < rowHeights.size(); j++)
			tempCol.push_back(&cells[j][i]);

		// Create the GridPanelColumns.
		GridPanelColumn col(tempCol);
		cols.push_back(col);
	}
}

void GridPanel::setX(float x)
{
	PControl::setX(x);

	for (size_t i = 0; i < cols.size(); i++)
	{
		if (i == 0)
			cols[i].setX(x);
		else
			cols[i].setX(cols[i - 1].getX() + cols[i - 1].getWidth());
	}
}

void GridPanel::setY(float y)
{
	PControl::setY(y);

	for (size_t i = 0; i < rows.size(); i++)
	{
		if (i == 0)
			rows[i].setY(y);
		else
			rows[i].setY(rows[i - 1].getY() + rows[i - 1].getHeight());
	}
}

void GridPanel::setWidth(float w)
{
	assert(w >= 0);

	// each row may have a different amount of H required from the scale, so loop through rows somehow.

	/*
	// Check if resizing is possible, based on fixed/min/max widths of columns
	// 1. Search for non-fixed cells, ignore fixed cells
	// 2. See if width changes can be applied to the cells

	float totalColMinWidth = 0.0f;
	float totalColMaxWidth = PConstants::PCONTROL_DEFAULT_MAXDIM;

	if (rows.size() > (size_t) 0)
	{
		totalColMinWidth = 0.0f;
		totalColMaxWidth = 0.0f;

		for (int i = 0; i < rows[0].getNumCols; i++)
		{
			totalColMinWidth += rows[0][i].getMinWidth();
			totalColMaxWidth += rows[0][i].getMaxWidth();
		}
	}

	// Check if resizing is possible, based on fixed/min/max widths of columns
	if (w >= totalColMinWidth && w <= totalColMaxWidth)
	{

	}
	*/

	PControl::setWidth(w);

	for (GridPanelRow& r : rows)
		r.setWidth(w);
}

void GridPanel::setHeight(float h)
{
	assert(h >= 0);

	// Check if resizing is possible, based on fixed/min/max heights of columns

	float rowY;

	for (size_t i = 0; i < rows.size(); i++)
	{
		rowY = i == 0 ? getY() : rowY + rows[i - 1].getHeight();

		rows[i].setY(rowY);

		// Maintain row proportions when we resize their heights.
		rows[i].setHeight((rows[i].getHeight() / getHeight()) * h);
	}

	PControl::setHeight(h);
}

void GridPanel::setRowHeight(int r, int h)
{
	assert(r >= 0 && r < (int) rows.size());

	if (h >= rows[r].getMinHeight() && h <= rows[r].getMaxHeight())
	{
		PControl::setHeight(getHeight() + (h - rows[r].getHeight()));

		for (size_t i = r; i < rows.size(); i++)
		{
			if (i == r)
				rows[r].setHeight(h);
			else
				rows[r].setY(rows[i - 1].getY() + rows[i - 1].getHeight());
		}
	}
}

GridPanelRowView& GridPanel::getRow(int r)
{
	// TODO: figure out a way to make this method const.
	assert(r >= 0 && r < rows.size());
	return rows[r].getView();
}

int GridPanel::getNumRows() const
{
	return rows.size();
}

void GridPanel::setColWidth(int c, int w)
{
	assert(c >= 0 && c < (int) cols.size());

	if (w >= cols[c].getMinWidth() && c <= cols[c].getMaxWidth())
	{
		PControl::setWidth(getWidth() + (w - cols[c].getWidth()));

		for (size_t i = c; i < cols.size(); i++)
		{
			if (i == c)
				cols[c].setWidth(w);
			else
				cols[c].setX(cols[i - 1].getX() + cols[i - 1].getWidth());
		}
	}
}

bool GridPanel::canResizeWidth(float w) const
{
	return false;
}

bool GridPanel::canResizeHeight(float h) const
{
	return false;
}

GridPanelColumnView& GridPanel::getColumn(int c)
{
	// TODO: figure out a way to make this method const.
	assert(c >= 0 && c < cols.size());
	return cols[c].getView();
}

int GridPanel::getNumCols() const
{
	return cols.size();
}

void GridPanel::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setColor(r, g, b);
}

void GridPanel::setBaseColor(float r, float g, float b)
{
	PControl::setBaseColor(r, g, b);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setBaseColor(r, g, b);
}

void GridPanel::setFocusColor(float r, float g, float b)
{
	PControl::setFocusColor(r, g, b);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setFocusColor(r, g, b);
}

void GridPanel::setColorState(PControlColorState s)
{
	PControl::setColorState(s);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setColorState(s);
}

void GridPanel::setColorStyle(PControlColorStyle s)
{
	PControl::setColorStyle(s);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setColorStyle(s);
}

void GridPanel::setColorSpeed(float seconds)
{
	PControl::setColorSpeed(seconds);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setColorSpeed(seconds);
}

void GridPanel::setAlpha(float a)
{
	PControl::setAlpha(a);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setAlpha(a);
}

void GridPanel::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setMinAlpha(a);
}

void GridPanel::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setMaxAlpha(a);
}

void GridPanel::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setFadeState(s);
}

void GridPanel::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setFadeStyle(s);
}

void GridPanel::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);

	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.setFadeSpeed(seconds);
}

void GridPanel::draw(SDL_Renderer* ren)
{
	for (std::vector<GridPanelCell>& row : cells)
		for (GridPanelCell& c : row)
			c.draw(ren);

	PControl::draw(ren);
}

GridPanelRowView& GridPanel::operator[](const int index)
{
	return getRow(index);
}

void GridPanel::handleEvent(Event* e)
{
	if (!e->handled)
	{

	}
}

bool GridPanel::canRowsScaleToWidth(float w) const
{
	float wdiff = w - getWidth();

	std::vector<const GridPanelRow*> freeRows;
	std::vector<const GridPanelRow*> wScaledRows;

	for (const GridPanelRow& r : rows)
		if (r.getStyle() == GridPanelRowStyle::FREE)
			freeRows.push_back(&r);
		else if (r.getStyle() == GridPanelRowStyle::SCALEDTOW)
			wScaledRows.push_back(&r);

	if (wScaledRows.size() == 0)
		return true;
	// If we have scale-to-width rows and free rows from which height may be added/removed, check if we can actually resize.
	else if (wScaledRows.size() > (size_t)0 && freeRows.size() > (size_t)0 && wdiff != 0)
	{
		float availableHeight = 0;
		float requiredHeight = 0;

		// Query available height we can add to/remove from rows with free heights.
		for (GridPanelRow* r : freeRows)
			if (wdiff > 0)
				availableHeight += r->getMaxHeight() - r->getHeight();
			else
				availableHeight += r->getHeight() - r->getMinHeight();

		// Query amount of height required by rows scaled to width.
		for (GridPanelRow* r : wScaledRows)
		{
			float hwratio = r->getHeight() / getWidth();

			requiredHeight += std::abs(w * hwratio - r->getHeight());
		}

		// Check if resizing row heights is possible.
		if (availableHeight >= requiredHeight)
		{
			// Increase row height from top to bottom.
			if (wdiff > 0)
				for (size_t i = 0; i < rows.size(); i++)
				{
					GridPanelRow& r = rows[i];

					if (r.getStyle() == GridPanelRowStyle::SCALEDTOW)
					{
						float hwratio = r.getHeight() / getWidth();
						setRowHeight(i, w * hwratio);
					}
					else if (r.getStyle() == GridPanelRowStyle::FREE)
					{
						if (requiredHeight > 0)
						{
							float heightUsed = r.getHeight() + requiredHeight >= r.getMaxHeight() ? r.getMaxHeight() - r.getHeight() : requiredHeight;
							setRowHeight(i, r.getHeight() + heightUsed);
							requiredHeight -= heightUsed;
						}
						else
							break;
					}
				}
			// Decrease row height from bottom to top.
			else
				for (int i = (int)rows.size() - 1; i >= 0; i--)
				{
					GridPanelRow& r = rows[i];

					if (r.getStyle() == GridPanelRowStyle::SCALEDTOW)
					{
						float hwratio = r.getHeight() / getWidth();
						setRowHeight(i, w * hwratio);
					}
					else if (r.getStyle() == GridPanelRowStyle::FREE)
					{
						if (requiredHeight > 0)
						{
							float heightUsed = r.getHeight() - requiredHeight <= r.getMinHeight() ? r.getHeight() - r.getMinHeight() : requiredHeight;
							setRowHeight(i, r.getHeight() - heightUsed);
							requiredHeight -= heightUsed;
						}
						else
							break;
					}
				}

			assert(requiredHeight == 0);
		}
	}

	return false;
}

bool GridPanel::canColsScaleToHeight(float h) const
{
	return false;
}