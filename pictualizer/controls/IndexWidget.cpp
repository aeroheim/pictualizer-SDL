#include "IndexWidget.h"

IndexWidget::IndexWidget(SDL_Renderer* ren, TTF_Font* font, int x, int y, int w, int h) :  
																		PControl(x, y, w, h),
																		oneGrid(x, y, w, h, 1, 1),
																		fourGrid(x, y, w, h, 2, 2),
																		nineGrid(x, y, w, h, 3, 3)
{
	this->ren = ren;
	this->font = font;
	index = 0;

	for (int i = 0; i < 9; i++)
	{
		Label digit(font, 0, 0, 0, 0);

		digit.setClipState(LabelClipState::CLIP);
		digit.setAlignState(LabelAlignState::CENTER);
		digitLabels.push_back(digit);
	}

	setIndex(0);
}

void IndexWidget::setIndex(int i)
{
	// TODO: remove max index limit
	assert(i < MAX_INDEX);

	int digit = i;
	int counter = 0;

	// nineGrid case or fourGrid case
	if (i >= 100)
	{
		GridPanel& nGrid = i >= 10000 ? nineGrid : fourGrid;

		for (int i = nGrid.getNumRows() - 1; i >= 0; i--)
			for (int j = nGrid[0].getNumCols() - 1; j >= 0; j--)
			{
				Label* digitLabel = &digitLabels[counter];

				digitLabel->setText(std::to_string(digit % 10), ren);
				nGrid[i][j].setElement(digitLabel);

				digit /= 10;
				++counter;
			}	
	}
	// oneGrid case
	else
	{
		Label* digitLabel = &digitLabels[counter];

		// Append an extra '0' to the label if necessary.
		if (digit < 10)
			digitLabel->setText("0" + std::to_string(digit), ren);
		else
			digitLabel->setText(std::to_string(digit), ren);
		
		oneGrid[0][0].setElement(digitLabel);
	}

	index = i;
}

int IndexWidget::getIndex()
{
	return index;
}

void IndexWidget::nextIndex()
{
	if (index + 1 < MAX_INDEX)
		setIndex(index + 1);
}

void IndexWidget::prevIndex()
{
	if (index - 1 >= 0)
		setIndex(index - 1);
}

void IndexWidget::setX(int x)
{
	this->x = x;
	oneGrid.setX(x);
	fourGrid.setX(x);
	nineGrid.setX(x);
}

void IndexWidget::setY(int y)
{
	this->y = y;
	oneGrid.setY(y);
	fourGrid.setY(y);
	nineGrid.setY(y);
}

void IndexWidget::setWidth(int w)
{
	this->w = w;
	oneGrid.setWidth(w);
	fourGrid.setWidth(w);
	nineGrid.setWidth(w);
}

void IndexWidget::setHeight(int h)
{
	this->h = h;
	oneGrid.setHeight(h);
	fourGrid.setHeight(h);
	nineGrid.setHeight(h);
}

void IndexWidget::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	for (Label& digitLabel : digitLabels)
		digitLabel.setColor(r, g, b);
}

SDL_Color* IndexWidget::getColor()
{
	return digitLabels[0].getColor();
}

void IndexWidget::setAlpha(Uint8 a)
{
	for (Label& digitLabel : digitLabels)
		digitLabel.setAlpha(a);
}

Uint8 IndexWidget::getAlpha()
{
	return digitLabels[0].getAlpha();
}

void IndexWidget::draw(SDL_Renderer* ren)
{
	GridPanel& nGrid = index >= 10000 ? nineGrid : (index >= 100 ? fourGrid : oneGrid);
	nGrid.draw(this->ren);
}
