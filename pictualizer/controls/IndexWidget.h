#pragma once

#include <vector>
#include <SDL_ttf.h>
#include "PControl.h"
#include "GridPanel.h"
#include "Label.h"

class IndexWidget : public PControl
{
	public:
		IndexWidget(SDL_Renderer* ren, TTF_Font* font, float x, float y, float w, float h);

		void setIndex(int i);
		int getIndex();

		void nextIndex();
		void prevIndex();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(Uint8 r, Uint8 g, Uint8 b);
		SDL_Color* getColor();

		void setAlpha(Uint8 a);
		Uint8 getAlpha();

		void draw(SDL_Renderer* ren = nullptr);

	private:
		SDL_Renderer* ren;

		GridPanel oneGrid;
		GridPanel fourGrid;
		GridPanel nineGrid;

		std::vector<Label> digitLabels;
		TTF_Font* font;
		int index;

		const int MAX_INDEX = 1000000000;
};