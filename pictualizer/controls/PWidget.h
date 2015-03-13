#pragma once

#include "PControl.h"
#include "PWidgetStates.h"
#include "../io/PCursors.h"
#include "../io/InputListener.h"
#include "../io/EventSubscriber.h"

class PWidget : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		void setMinWidth(float minWidth);
		void setMinHeight(float minHeight);

		void setResizeState(PWidgetResizeState s);
		PWidgetResizeState getResizeState();

		void setColor(float r, float g, float b);
		void setAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeDelta(float delta);

		virtual void draw(SDL_Renderer* ren) = 0;
		virtual void handleEvent(Event* e);

	protected:
		PWidget(float x, float y, float w, float h);
		virtual ~PWidget();

		float minWidth;
		float minHeight;

	private:
		SDL_Keycode IGNORE_KEY = SDLK_LCTRL;

		const int DRAG_ZONE_DIST = 15;
		PWidgetResizeState resizeState;
		bool dragResizing;
		bool dragging;

		bool widgetIntersects(float x, float y, float w, float h);

		void setDragCursor(MouseMotionEvent* e);
		void getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH);
		void OnMouseMotion(MouseMotionEvent* e);

};