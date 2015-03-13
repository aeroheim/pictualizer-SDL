#include "PWidget.h"
#include <iostream>

using namespace std;

PWidget::PWidget(float x, float y, float w, float h) : PControl(x, y, w, h)
{
	dragging = false;
	dragResizing = false;
	resizeState = PWidgetResizeState::FREE;
	minWidth = 20;
	minHeight = 20;

	registerKey(IGNORE_KEY);
}

PWidget::~PWidget() {}

void PWidget::setMinWidth(float minWidth)
{
	this->minWidth = minWidth;
}

void PWidget::setMinHeight(float minHeight)
{
	this->minHeight = minHeight;
}

void PWidget::setResizeState(PWidgetResizeState s)
{
	resizeState = s;
}

PWidgetResizeState PWidget::getResizeState()
{
	return resizeState;
}

void PWidget::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
}

void PWidget::setAlpha(float a)
{
	PControl::setAlpha(a);
}

void PWidget::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);
}

void PWidget::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);
}

void PWidget::draw(SDL_Renderer* = nullptr)
{
	PControl::draw(nullptr);
}

void PWidget::handleEvent(Event* e)
{
	if (!e->handled)
	{
		if (KeyDownEvent* keyDownEvent = dynamic_cast<KeyDownEvent*>(e))
			setKeyHeld(keyDownEvent->key);
		else if (KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e))
		{
			setKeyReleased(keyUpEvent->key);

			if (keyUpEvent->key == IGNORE_KEY)
			{
				SDL_ShowCursor(SDL_ENABLE);
				SDL_SetRelativeMouseMode(SDL_FALSE);
			}
		}
		else if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
		{
			if (!keyHeld(IGNORE_KEY))
			{
				if (mouseInside(mouseDownEvent->x, mouseDownEvent->y) && !lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
				{
					lMouseHeld = true;
					mDownX = mouseDownEvent->x;
					mDownY = mouseDownEvent->y;

					if (SDL_GetCursor() != PCursors::ARROW)
						dragResizing = true;
					else
						dragging = true;
				}
			}
		}
		else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
		{
			if (mouseUpEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = false;
				dragResizing = false;
				dragging = false;
			}
		}
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		{
			if (!keyHeld(IGNORE_KEY))
			{
				if (!dragResizing && !dragging && mouseInside(mouseMotionEvent->x, mouseMotionEvent->y))
				{
					setDragCursor(mouseMotionEvent);
					e->handled = true;
				}

				if (lMouseHeld)
				{
					OnMouseMotion(mouseMotionEvent);
					e->handled = true;
				}
			}
		}
		else if (WidgetMoveEvent* widgetMoveEvent = dynamic_cast<WidgetMoveEvent*>(e))
		{
			// If a widget move event will collide with us, handle the event to prevent it from moving.
			if (widgetMoveEvent->widget != this && widgetIntersects(widgetMoveEvent->x, widgetMoveEvent->y, widgetMoveEvent->widget->getWidth(), widgetMoveEvent->widget->getHeight()))
				e->handled = true;
		}
		else if (WidgetResizeEvent* widgetResizeEvent = dynamic_cast<WidgetResizeEvent*>(e))
		{
			// If a widget reszie event will collide with us, handle the event to prevent it from resizing.
			if (widgetResizeEvent->widget != this && widgetIntersects(widgetResizeEvent->x, widgetResizeEvent->y, widgetResizeEvent->w, widgetResizeEvent->h))
				e->handled = true;
		}
	}
}

bool PWidget::widgetIntersects(float x, float y, float w, float h)
{
	return ((getRoundedX() < x + w) && (getRoundedX() + getRoundedWidth() > x) 
			&& (getRoundedY() < y + h) && (getRoundedY() + getRoundedHeight() > y));
}

void PWidget::setDragCursor(MouseMotionEvent* e)
{
	if ((e->x < getRoundedX() + DRAG_ZONE_DIST) && (e->y < getRoundedY() + DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENW);
	else if ((e->x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST) && (e->y < getRoundedY() + DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENE);
	else if ((e->x < getRoundedX() + DRAG_ZONE_DIST) && (e->y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESW);
	else if ((e->x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST) && (e->y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESE);
	else if (e->y < getRoundedY() + DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEN);
	else if (e->y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZES);
	else if (e->x < getRoundedX() + DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEW);
	else if (e->x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEE);
	else
		SDL_SetCursor(PCursors::ARROW);
}

void PWidget::getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH)
{
	SDL_Cursor* cursor = SDL_GetCursor();

	if (cursor == PCursors::SIZENW)
	{
		*newX = getX() + e->xrel;
		*newY = getY() + e->yrel;
		*newW = getWidth() - e->xrel;
		*newH = getHeight() - e->yrel;
	}
	else if (cursor == PCursors::SIZENE)
	{
		*newX = getX();
		*newY = getY() + e->yrel;
		*newW = getWidth() + e->xrel;
		*newH = getHeight() - e->yrel;
	}
	else if (cursor == PCursors::SIZESW)
	{
		*newX = getX() + e->xrel;
		*newY = getY();
		*newW = getWidth() - e->xrel;
		*newH = getHeight() + e->yrel;
	}
	else if (cursor == PCursors::SIZESE)
	{
		*newX = getX();
		*newY = getY();
		*newW = getWidth() + e->xrel;
		*newH = getHeight() + e->yrel;
	}
	else if (cursor == PCursors::SIZEN)
	{
		*newX = getX();
		*newY = getY() + e->yrel;
		*newW = getWidth();
		*newH = getHeight() - e->yrel;
	}
	else if (cursor == PCursors::SIZES)
	{
		*newX = getX();
		*newY = getY();
		*newW = getWidth();
		*newH = getHeight() + e->yrel;
	}
	else if (cursor == PCursors::SIZEW)
	{
		*newX = getX() + e->xrel;
		*newY = getY();
		*newW = getWidth() - e->xrel;
		*newH = getHeight();
	}
	else if (cursor == PCursors::SIZEE)
	{
		*newX = getX();
		*newY = getY();
		*newW = getWidth() + e->xrel;
		*newH = getHeight();
	}
}

void PWidget::OnMouseMotion(MouseMotionEvent* e)
{
	// Drag the widget around.
	if (SDL_GetCursor() == PCursors::ARROW)
	{
		float newX = getX() + e->xrel;
		float newY = getY() + e->yrel;

		WidgetMoveEvent widgetMoveEvent(this, newX, newY);
		notify(&widgetMoveEvent);

		// If no widget collision, we can move the widget around.
		if (!widgetMoveEvent.handled)
		{
			setX(newX);
			setY(newY);
		}
	}
	// Drag resize the widget.
	else
	{
		// TODO: support SCALED resizing.
		float newX, newY, newW, newH;

		getDragValues(e, &newX, &newY, &newW, &newH);

		if (newW > minWidth && newH > minHeight)
		{
			WidgetResizeEvent widgetResizeEvent(this, newX, newY, newW, newH);
			notify(&widgetResizeEvent);

			// If no widget collision, we can resize the widget.
			if (!widgetResizeEvent.handled)
			{
				setX(newX);
				setY(newY);
				setWidth(newW);
				setHeight(newH);
			}
		}
	}
}