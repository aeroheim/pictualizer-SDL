#pragma once

#include "Image.h"
#include "PControl.h"
#include "PWidgetStates.h"
#include "../io/InputListener.h"
#include "../io/EventSubscriber.h"

class PWidget : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);

		float getInnerX() const;
		float getInnerY() const;
		float getInnerWidth() const;
		float getInnerHeight() const;

		void setBackgroundAlpha(float a);
		void setBackgroundMinAlpha(float a);
		void setBackgroundMaxAlpha(float a);
		void setBackgroundFadeState(PControlFadeState s);
		void setBackgroundFadeStyle(PControlFadeStyle s);
		void setBackgroundFadeSpeed(float seconds);
		float getBackgroundAlpha() const;
		float getBackgroundMinAlpha() const;
		float getBackgroundMaxAlpha() const;
		PControlFadeState getBackgroundFadeState() const;
		PControlFadeStyle getBackgroundFadeStyle() const;
		float getBackgroundFadeSpeed() const;

		void setBackgroundColor(float r, float g, float b);
		void setBackgroundColor(const PFloatColor& c);
		void setBackgroundColor(const PIntColor& c);
		const PFloatColor& getBackgroundColor() const;
		PIntColor getRoundedBackgroundColor() const;

		void setResizeState(PWidgetResizeState s);
		PWidgetResizeState getResizeState();

		virtual void draw(SDL_Renderer* ren) = 0;

		virtual void handleEvent(Event* e);

	protected:
		PWidget(SDL_Renderer* ren, float x, float y, float w, float h);
		virtual ~PWidget();

	private:
		Image background;

		PWidgetResizeState resizeState;
		bool mouseOver;
		bool dragResizing;
		bool dragging;

		float minWidth;
		float minHeight;

		bool widgetIntersects(float x, float y, float w, float h);
		void setDragCursor(int x, int y);
		void getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH);
		void OnMouseMotion(MouseMotionEvent* e);
};