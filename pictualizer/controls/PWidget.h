#pragma once

#include "PControl.h"
#include "PWidgetStates.h"
#include "../io/PCursors.h"
#include "../io/InputListener.h"
#include "../io/EventSubscriber.h"

class PWidget : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		void setMinWidth(int minWidth);
		void setMinHeight(int minHeight);

		void setResizeState(PWidgetResizeState s);
		PWidgetResizeState getResizeState();

		virtual void draw(SDL_Renderer* ren) = 0;
		virtual void handleEvent(Event* e);

	protected:
		PWidget(int x, int y, int w, int h);
		virtual ~PWidget();

		bool posInWidget(int x, int y);

		int minWidth;
		int minHeight;

	private:
		SDL_Keycode IGNORE_KEY = SDLK_LCTRL;

		const int DRAG_ZONE_DIST = 15;
		PWidgetResizeState resizeState;
		bool dragResizing;
		bool dragging;

		bool widgetIntersects(PWidget* widget);

		void setDragCursor(MouseMotionEvent* e);
		void getDragValues(MouseMotionEvent* e, int* newX, int* newY, int* newW, int* newH);
		void OnMouseMotion(MouseMotionEvent* e);

};