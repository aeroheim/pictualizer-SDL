#pragma once

#include "PControl.h"
#include "PWidgetStates.h"
#include "../io/PCursors.h"
#include "../io/InputListener.h"
#include "../io/EventSubscriber.h"

class PWidget : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		float getInnerX();
		float getInnerY();
		float getInnerWidth();
		float getInnerHeight();

		void setMinWidth(float minWidth);
		void setMinHeight(float minHeight);
		float getMinWidth();
		float getMinHeight();

		void setResizeState(PWidgetResizeState s);
		PWidgetResizeState getResizeState();

		virtual void handleEvent(Event* e);

	protected:
		PWidget(float x, float y, float w, float h);
		virtual ~PWidget();

	private:
		SDL_Keycode IGNORE_KEY = SDLK_LCTRL;

		const int DRAG_ZONE_DIST = 15;
		PWidgetResizeState resizeState;
		bool dragResizing;
		bool dragging;

		float minWidth;
		float minHeight;

		bool widgetIntersects(float x, float y, float w, float h);

		void setDragCursor(MouseMotionEvent* e);
		void getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH);
		void OnMouseMotion(MouseMotionEvent* e);

};