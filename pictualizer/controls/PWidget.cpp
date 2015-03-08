#include "PWidget.h"
#include <iostream>

using namespace std;

PWidget::PWidget(int x, int y, int w, int h) : PControl(x, y, w, h)
{
	dragging = false;
	dragResizing = false;
	resizeState = PWidgetResizeState::FREE;
	registerKey(IGNORE_KEY);
}

PWidget::~PWidget() {}

void PWidget::setMinWidth(int minWidth)
{
	this->minWidth = minWidth;
}

void PWidget::setMinHeight(int minHeight)
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
				if (posInWidget(mouseDownEvent->x, mouseDownEvent->y) && !lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
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
				if (!dragResizing && !dragging && posInWidget(mouseMotionEvent->x, mouseMotionEvent->y))
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
			if (widgetMoveEvent->widget != this && widgetIntersects(widgetMoveEvent->widget))
				e->handled = true;
		}
		else if (WidgetResizeEvent* widgetResizeEvent = dynamic_cast<WidgetResizeEvent*>(e))
		{
			// If a widget reszie event will collide with us, handle the event to prevent it from resizing.
			if (widgetResizeEvent->widget != this && widgetIntersects(widgetMoveEvent->widget))
				e->handled = true;
		}
	}
}

bool PWidget::posInWidget(int x, int y)
{
	return ((x >= this->x && x <= this->x + w) && (y >= this->y && y <= this->y + h));
}

bool PWidget::widgetIntersects(PWidget* widget)
{
	return ((x < widget->getX() + widget->getWidth()) && (x + w > widget->getX()) && (y < widget->getY() + widget->getHeight()) && (y + h > widget->getY()));
}

void PWidget::setDragCursor(MouseMotionEvent* e)
{
	if ((e->x < x + DRAG_ZONE_DIST) && (e->y < y + DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENW);
	else if ((e->x > x + w - DRAG_ZONE_DIST) && (e->y < y + DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENE);
	else if ((e->x < x + DRAG_ZONE_DIST) && (e->y > y + h - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESW);
	else if ((e->x > x + w - DRAG_ZONE_DIST) && (e->y > y + h - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESE);
	else if (e->y < y + DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEN);
	else if (e->y > y + h - DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZES);
	else if (e->x < x + DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEW);
	else if (e->x > x + w - DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEE);
	else
		SDL_SetCursor(PCursors::ARROW);
}

void PWidget::getDragValues(MouseMotionEvent* e, int* newX, int* newY, int* newW, int* newH)
{
	SDL_Cursor* cursor = SDL_GetCursor();

	if (cursor == PCursors::SIZENW)
	{
		*newX = x + e->xrel;
		*newY = y + e->yrel;
		*newW = w - e->xrel;
		*newH = h - e->yrel;
	}
	else if (cursor == PCursors::SIZENE)
	{
		*newX = x;
		*newY = y + e->yrel;
		*newW = w + e->xrel;
		*newH = h - e->yrel;
	}
	else if (cursor == PCursors::SIZESW)
	{
		*newX = x + e->xrel;
		*newY = y;
		*newW = w - e->xrel;
		*newH = h + e->yrel;
	}
	else if (cursor == PCursors::SIZESE)
	{
		*newX = x;
		*newY = y;
		*newW = w + e->xrel;
		*newH = h + e->yrel;
	}
	else if (cursor == PCursors::SIZEN)
	{
		*newX = x;
		*newY = y + e->yrel;
		*newW = w;
		*newH = h - e->yrel;
	}
	else if (cursor == PCursors::SIZES)
	{
		*newX = x;
		*newY = y;
		*newW = w;
		*newH = h + e->yrel;
	}
	else if (cursor == PCursors::SIZEW)
	{
		*newX = x + e->xrel;
		*newY = y;
		*newW = w - e->xrel;
		*newH = h;
	}
	else if (cursor == PCursors::SIZEE)
	{
		*newX = x;
		*newY = y;
		*newW = w + e->xrel;
		*newH = h;
	}
}

void PWidget::OnMouseMotion(MouseMotionEvent* e)
{
	// Drag the widget around.
	if (SDL_GetCursor() == PCursors::ARROW)
	{
		int newX = x + e->xrel;
		int newY = y + e->yrel;

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
		int newX, newY, newW, newH;

		getDragValues(e, &newX, &newY, &newW, &newH);

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