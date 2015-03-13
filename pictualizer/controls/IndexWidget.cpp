#include "IndexWidget.h"

IndexWidget::IndexWidget(SDL_Renderer* ren, TTF_Font* font, float x, float y, float w, float h) :  
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

void IndexWidget::setX(float x)
{
	PControl::setX(x);
	oneGrid.setX(x);
	fourGrid.setX(x);
	nineGrid.setX(x);
}

void IndexWidget::setY(float y)
{
	PControl::setY(y);
	oneGrid.setY(y);
	fourGrid.setY(y);
	nineGrid.setY(y);
}

void IndexWidget::setWidth(float w)
{
	PControl::setWidth(w);
	oneGrid.setWidth(w);
	fourGrid.setWidth(w);
	nineGrid.setWidth(w);
}

void IndexWidget::setHeight(float h)
{
	PControl::setHeight(h);
	oneGrid.setHeight(h);
	fourGrid.setHeight(h);
	nineGrid.setHeight(h);
}

void IndexWidget::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	for (Label& digitLabel : digitLabels)
		digitLabel.setColor(r, g, b);
}


void IndexWidget::setAlpha(float a)
{
	PControl::setAlpha(a);

	for (Label& digitLabel : digitLabels)
		digitLabel.setAlpha(a);
}

void IndexWidget::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);

	oneGrid.setFadeState(s);
	fourGrid.setFadeState(s);
	nineGrid.setFadeState(s);

	for (Label& digitLabel : digitLabels)
		digitLabel.setFadeState(s);
}

void IndexWidget::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);

	oneGrid.setFadeDelta(delta);
	fourGrid.setFadeDelta(delta);
	nineGrid.setFadeDelta(delta);

	for (Label& digitLabel : digitLabels)
		digitLabel.setFadeDelta(delta);
}

void IndexWidget::draw(SDL_Renderer* ren)
{
	GridPanel& nGrid = index >= 10000 ? nineGrid : (index >= 100 ? fourGrid : oneGrid);

	if (ren)
		nGrid.draw(ren);
	else
		nGrid.draw(this->ren);

	PControl::draw(nullptr);
}
