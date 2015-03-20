#pragma once

#include "Image.h"
#include "PControl.h"
#include "PWidgetStates.h"
#include "../io/PCursors.h"
#include "../io/InputListener.h"
#include "../io/EventSubscriber.h"

class PWidget : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);

		float getInnerX();
		float getInnerY();
		float getInnerWidth();
		float getInnerHeight();

		void setMinWidth(float minWidth);
		void setMinHeight(float minHeight);
		float getMinWidth();
		float getMinHeight();

		void setBackgroundAlpha(float a);
		void setBackgroundMinAlpha(float a);
		void setBackgroundMaxAlpha(float a);
		void setBackgroundFadeState(PControlFadeState s);
		void setBackgroundFadeDelta(float delta);

		void setResizeState(PWidgetResizeState s);
		PWidgetResizeState getResizeState();

		virtual void draw(SDL_Renderer* ren) = 0;

		virtual void handleEvent(Event* e);

	protected:
		PWidget(SDL_Renderer* ren, float x, float y, float w, float h);
		virtual ~PWidget();

	private:
		SDL_Keycode IGNORE_KEY = SDLK_LCTRL;

		Image background;

		const int DRAG_ZONE_DIST = 12;
		PWidgetResizeState resizeState;
		bool mouseOver;
		bool dragResizing;
		bool dragging;

		float minWidth;
		float minHeight;

		bool widgetIntersects(float x, float y, float w, float h);
		void setDragCursor(MouseMotionEvent* e);
		void getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH);
		void OnMouseMotion(MouseMotionEvent* e);
};