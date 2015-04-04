#pragma once

#include <vector>
#include <SDL_ttf.h>
#include "PControl.h"
#include "GridPanel.h"
#include "Label.h"
#include "../assets/fonts/PFonts.h"

class IndexWidget : public PControl
{
	public:
		IndexWidget(SDL_Renderer* ren, PFontType fontType, float x, float y, float w, float h);

		void setIndex(int i);
		int getIndex();

		void nextIndex();
		void prevIndex();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);

		void setTint(float t);
		void setBaseTint(float t);
		void setFocusTint(float t);
		void setTintState(PControlTintState s);
		void setTintDelta(float delta);

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeDelta(float delta);

		void draw(SDL_Renderer* ren);

	private:
		GridPanel oneGrid;
		GridPanel fourGrid;
		GridPanel nineGrid;

		std::vector<Label> digitLabels;
		PFontType fontType;
		int index;

		const int MAX_INDEX = 1000000000;
};