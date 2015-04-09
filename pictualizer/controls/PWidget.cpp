#include "PWidget.h"

PWidget::PWidget(SDL_Renderer* ren, float x, float y, float w, float h) :
	PControl(x, y, w, h),
	background(x, y, w, h),
	dragging(false),
	dragResizing(false),
	mouseOver(false),
	resizeState(PWidgetResizeState::SCALED),
	minWidth(20),
	minHeight(20)
{
	SDL_Surface* bgSurface = SDL_CreateRGBSurface(0, (int) std::round(w), (int) std::round(h), 32, 0, 0, 0, 0);
	SDL_FillRect(bgSurface, NULL, SDL_MapRGBA(bgSurface->format, 0, 0, 0, 255));
	
	SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(ren, bgSurface);
	SDL_SetTextureBlendMode(bgTexture, SDL_BLENDMODE_BLEND);
	background.setImage(bgTexture);

	SDL_FreeSurface(bgSurface);
}

PWidget::~PWidget() {}

void PWidget::setX(float x)
{
	PControl::setX(x);
	background.setX(x);
}

void PWidget::setY(float y)
{
	PControl::setY(y);
	background.setY(y);
}

void PWidget::setWidth(float w)
{
	PControl::setWidth(w);
	background.setWidth(w);
}

void PWidget::setHeight(float h)
{
	PControl::setHeight(h);
	background.setHeight(h);
}

float PWidget::getInnerX()
{
	return getX() + DRAG_ZONE_DIST;
}

float PWidget::getInnerY()
{
	return getY() + DRAG_ZONE_DIST;
}

float PWidget::getInnerWidth()
{
	return getWidth() - (DRAG_ZONE_DIST * 2);
}

float PWidget::getInnerHeight()
{
	return getHeight() - (DRAG_ZONE_DIST * 2);
}

void PWidget::setMinWidth(float minWidth)
{
	assert(minWidth >= 0);

	this->minWidth = minWidth;
}

void PWidget::setMinHeight(float minHeight)
{
	assert(minHeight >= 0);

	this->minHeight = minHeight;
}

float PWidget::getMinWidth()
{
	return minWidth;
}

float PWidget::getMinHeight()
{
	return minHeight;
}

void PWidget::setBackgroundAlpha(float a)
{
	background.setAlpha(a);
}

void PWidget::setBackgroundMinAlpha(float a)
{
	background.setMinAlpha(a);
}

void PWidget::setBackgroundMaxAlpha(float a)
{
	background.setMaxAlpha(a);
}

void PWidget::setBackgroundFadeState(PControlFadeState s)
{
	background.setFadeState(s);
}

void PWidget::setBackgroundFadeDelta(float delta)
{
	background.setFadeDelta(delta);
}

void PWidget::setResizeState(PWidgetResizeState s)
{
	resizeState = s;
}

PWidgetResizeState PWidget::getResizeState()
{
	return resizeState;
}

void PWidget::draw(SDL_Renderer* ren)
{
	background.draw(ren);
	PControl::draw(ren);
}

void PWidget::handleEvent(Event* e)
{
	notify(e);

	if (!e->handled)
	{
		if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
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

				e->handled = true;
			}
		}
		else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
		{
			if (mouseUpEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = false;
				dragResizing = false;
				dragging = false;

				setDragCursor(mouseUpEvent->x, mouseUpEvent->y);
			}
		}
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		{
			if (!dragResizing && !dragging && mouseInside(mouseMotionEvent->x, mouseMotionEvent->y))
			{
				setDragCursor(mouseMotionEvent->x, mouseMotionEvent->y);
				e->handled = true;
			}

			if (lMouseHeld)
				e->handled = true;

			PWidget::OnMouseMotion(mouseMotionEvent);
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

void PWidget::setDragCursor(int x, int y)
{
	if ((x < getRoundedX() + DRAG_ZONE_DIST && x >= getRoundedX()) && (y < getRoundedY() + DRAG_ZONE_DIST && y >= getRoundedY()))
		SDL_SetCursor(PCursors::SIZENW);
	else if ((x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST && x <= getRoundedX() + getRoundedWidth()) && (y < getRoundedY() + DRAG_ZONE_DIST && y >= getRoundedY()))
		SDL_SetCursor(PCursors::SIZENE);
	else if ((x < getRoundedX() + DRAG_ZONE_DIST && y >= getRoundedY()) && (y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST && y <= getRoundedY() + getRoundedHeight()))
		SDL_SetCursor(PCursors::SIZESW);
	else if ((x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST && x <= getRoundedX() + getRoundedWidth()) && (y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST && y <= getRoundedY() + getRoundedHeight()))
		SDL_SetCursor(PCursors::SIZESE);
	else if (y < getRoundedY() + DRAG_ZONE_DIST && y >= getRoundedY())
		SDL_SetCursor(PCursors::SIZEN);
	else if (y > getRoundedY() + getRoundedHeight() - DRAG_ZONE_DIST && y <= getRoundedY() + getRoundedHeight())
		SDL_SetCursor(PCursors::SIZES);
	else if (x < getRoundedX() + DRAG_ZONE_DIST && x >= getRoundedX())
		SDL_SetCursor(PCursors::SIZEW);
	else if (x > getRoundedX() + getRoundedWidth() - DRAG_ZONE_DIST && x <= getRoundedX() + getRoundedWidth())
		SDL_SetCursor(PCursors::SIZEE);
	else
		SDL_SetCursor(PCursors::ARROW);
}

void PWidget::getDragValues(MouseMotionEvent* e, float* newX, float* newY, float* newW, float* newH)
{
	SDL_Cursor* cursor = SDL_GetCursor();

	if (cursor == PCursors::SIZENW)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();
			float xrel = e->xrel * SMOOTH_RESIZE_SCALE;
			float yrel = e->yrel * SMOOTH_RESIZE_SCALE;

			// Apply x movement
			*newX = getX() + xrel;
			*newY = getY() + (xrel / whratio);
			*newW = getWidth() - xrel;
			*newH = getHeight() - (xrel / whratio);

			// Apply y movement
			*newX += yrel * whratio;
			*newY += yrel;
			*newW -= yrel * whratio;
			*newH -= yrel;
		}
		else
		{
			*newX = getX() + e->xrel;
			*newY = getY() + e->yrel;
			*newW = getWidth() - e->xrel;
			*newH = getHeight() - e->yrel;
		}
	}
	else if (cursor == PCursors::SIZENE)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();
			float xrel = e->xrel * SMOOTH_RESIZE_SCALE;
			float yrel = e->yrel * SMOOTH_RESIZE_SCALE;

			*newX = getX();

			// Apply x movement
			*newY = getY() - (xrel / whratio);
			*newH = getHeight() + (xrel / whratio);
			*newW = getWidth() + xrel;

			// Apply y movement
			*newY += yrel;
			*newH -= yrel;
			*newW -= yrel * whratio;
		}
		else
		{
			*newX = getX();
			*newY = getY() + e->yrel;
			*newW = getWidth() + e->xrel;
			*newH = getHeight() - e->yrel;
		}
	}
	else if (cursor == PCursors::SIZESW)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();
			float xrel = e->xrel * SMOOTH_RESIZE_SCALE;
			float yrel = e->yrel * SMOOTH_RESIZE_SCALE;

			*newY = getY();

			// Apply x movement
			*newX = getX() + xrel;
			*newW = getWidth() - xrel;
			*newH = getHeight() - (xrel / whratio);

			// Apply y movement
			*newX -= yrel * whratio;
			*newW += yrel * whratio;
			*newH += yrel;
		}
		else
		{
			*newX = getX() + e->xrel;
			*newY = getY();
			*newW = getWidth() - e->xrel;
			*newH = getHeight() + e->yrel;
		}
	}
	else if (cursor == PCursors::SIZESE)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();
			float xrel = e->xrel * SMOOTH_RESIZE_SCALE;
			float yrel = e->yrel * SMOOTH_RESIZE_SCALE;

			*newX = getX();
			*newY = getY();

			// Apply x movement
			*newW = getWidth() + xrel;
			*newH = getHeight() + xrel / whratio;

			// Apply y movement
			*newW += yrel * whratio;
			*newH += yrel;
		}
		else
		{
			*newX = getX();
			*newY = getY();
			*newW = getWidth() + e->xrel;
			*newH = getHeight() + e->yrel;
		}
	}
	else if (cursor == PCursors::SIZEN)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();

			*newX = getX();
			*newY = getY() + e->yrel;
			*newW = getWidth() - (e->yrel * whratio);
			*newH = getHeight() - e->yrel;
		}
		else
		{
			*newX = getX();
			*newY = getY() + e->yrel;
			*newW = getWidth();
			*newH = getHeight() - e->yrel;
		}
	}
	else if (cursor == PCursors::SIZES)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();

			*newX = getX();
			*newY = getY();
			*newW = getWidth() + (e->yrel * whratio);
			*newH = getHeight() + e->yrel;
		}
		else
		{
			*newX = getX();
			*newY = getY();
			*newW = getWidth();
			*newH = getHeight() + e->yrel;
		}
	}
	else if (cursor == PCursors::SIZEW)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();

			*newX = getX() + e->xrel;
			*newY = getY();
			*newW = getWidth() - e->xrel;
			*newH = getHeight() - (e->xrel / whratio);
		}
		else
		{
			*newX = getX() + e->xrel;
			*newY = getY();
			*newW = getWidth() - e->xrel;
			*newH = getHeight();
		}
	}
	else if (cursor == PCursors::SIZEE)
	{
		if (resizeState == PWidgetResizeState::SCALED)
		{
			float whratio = getWidth() / getHeight();

			*newX = getX();
			*newY = getY();
			*newW = getWidth() + e->xrel;
			*newH = getHeight() + (e->xrel / whratio);
		}
		else
		{
			*newX = getX();
			*newY = getY();
			*newW = getWidth() + e->xrel;
			*newH = getHeight();
		}
	}
}

void PWidget::OnMouseMotion(MouseMotionEvent* e)
{
	bool mouseOverWidget = mouseInside(e->x, e->y);

	if (!mouseOver && mouseOverWidget)
	{
		mouseOver = true;
		setBackgroundFadeState(PControlFadeState::FADEIN);
	}
	else if (!mouseOverWidget)
	{
		mouseOver = false;
		setBackgroundFadeState(PControlFadeState::FADEOUT);
	}

	if (lMouseHeld)
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
}