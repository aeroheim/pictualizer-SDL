#pragma once

#include <SDL.h>
#include <cassert>
#include "PControl.h"

class GridPanelCell : public PControl
{
	public:
		GridPanelCell(float x, float y, float w, float h);
		GridPanelCell(PControl* e, float x, float y, float w, float h);

		void setElement(PControl* e);
		PControl* getElement();

		void setPadding(float l, float u, float r, float d);

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
		PControl* element;
		float padding[4];
};