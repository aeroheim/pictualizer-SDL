#pragma once

#include <SDL.h>
#include <vector>
#include <cassert>
#include "PControl.h"
#include "GridPanelCellView.h"
#include "GridPanelStates.h"

class GridPanelCell : public PControl
{
	public:
		GridPanelCell(float x, float y, float w, float h);
		GridPanelCell(const GridPanelCell& other);

		GridPanelCellView& getView();

		void setElement(PControl* e);
		PControl* getElement() const;

		void setPadding(float l, float u, float r, float d);
		std::vector<float> getPadding() const;

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		bool canResize(float w, float h) const;

		void setColor(float r, float g, float b);
		void setBaseColor(float r, float g, float b);
		void setFocusColor(float r, float g, float b);
		void setColorState(PControlColorState s);
		void setColorStyle(PControlColorStyle s);
		void setColorSpeed(float seconds);

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeStyle(PControlFadeStyle s);
		void setFadeSpeed(float speed);

		void draw(SDL_Renderer* ren);

	private:
		GridPanelCellView view;
		PControl* element;
		float padding[4];
};