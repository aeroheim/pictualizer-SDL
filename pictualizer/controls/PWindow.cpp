#pragma once

#include "PWindow.h"
#include "../assets/cursors/PCursors.h"
#include "../util/PUtils.h"

PWindow::PWindow(std::string windowName, int windowX, int windowY, int windowWidth, int windowHeight) : ControlPresenter(windowX, windowY, windowWidth, windowHeight) 
{
	// Create SDL window.
	_window = SDL_CreateWindow(windowName.c_str(), windowX, windowY, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	if (_window == nullptr)
	{
		std::string error(SDL_GetError());
		throw "Window Initialization Error: " + error;
	}

	// Create SDL renderer.
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (_renderer == nullptr)
	{
		SDL_DestroyWindow(_window);
		std::string error(SDL_GetError());
		throw "Renderer Initialization Error: " + error;
	}

	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);	// Use blending to allow for alpha effects on textures.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");		// Use linear filtering to preserve image quality when scaled.

	ControlPresenter::setX(windowX);
	ControlPresenter::setY(windowY);
	ControlPresenter::setWidth(windowWidth);
	ControlPresenter::setHeight(windowHeight);
}

PWindow::~PWindow()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
}

void PWindow::setX(float x)
{
	ControlPresenter::setX(x);
	SDL_SetWindowPosition(_window, x, ControlPresenter::getY());
}

void PWindow::setY(float y)
{
	ControlPresenter::setY(y);
	SDL_SetWindowPosition(_window, ControlPresenter::getX(), y);
}

void PWindow::setWidth(float w)
{
	ControlPresenter::setWidth(w);
	SDL_SetWindowSize(_window, w, ControlPresenter::getHeight());
}

void PWindow::setHeight(float h)
{
	ControlPresenter::setHeight(h);
	SDL_SetWindowSize(_window, ControlPresenter::getWidth(), h);
}

void PWindow::moveTo(float x, float y)
{
	ControlPresenter::moveTo(x, y);
	SDL_SetWindowPosition(_window, x, y);
}

void PWindow::resize(float w, float h)
{
	ControlPresenter::resize(w, h);
	SDL_SetWindowSize(_window, w, h);
}

void PWindow::draw(SDL_Renderer* ren = nullptr)
{
	ControlPresenter::draw(ren == nullptr ? _renderer : ren);
}

void PWindow::handleEvent(Event* e)
{
	notify(e);

	if (auto mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
	{
		OnMouseDown(*mouseDownEvent);
	}
	else if (auto mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
	{
		OnMouseUp(*mouseUpEvent);
	}
	else if (auto mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
	{
		OnMouseMotion(*mouseMotionEvent);
	}
	else if (auto mouseDragEvent = dynamic_cast<MouseDragEvent*>(e))
	{
		OnMouseDrag(*mouseDragEvent);
	}
}

void PWindow::OnMouseDown(MouseDownEvent& e)
{
	if (!e.handled && e.button == SDL_BUTTON_LEFT)
	{
		// Capture global mouse events and deny mouse input to other windows for dragging.
		SDL_CaptureMouse(SDL_TRUE);

		SDL_GetGlobalMouseState(&_globalMouseDownX, &_globalMouseDownY);
		_resizeStartWindowWidth = ControlPresenter::getWidth();
		_resizeStartWindowHeight = ControlPresenter::getHeight();

		if (mouseDownInResizeZone(e.x, e.y))
		{
			_dragResizing = true;
		}
	}
}

void PWindow::OnMouseUp(MouseUpEvent& e)
{
	if (!e.handled && e.button == SDL_BUTTON_LEFT)
	{
		// Capture registering global mouse events.
		SDL_CaptureMouse(SDL_FALSE);

		_dragResizing = false;
	}
}

void PWindow::OnMouseMotion(MouseMotionEvent& e) const
{
	if (!e.handled)
	{
		setResizeCursor(e);
		e.handled = true;
	}
}

void PWindow::OnMouseDrag(MouseDragEvent& e)
{
	if (!e.handled)
	{
		if (_dragResizing)
		{
			dragResize();
		}
		else
		{
			SDL_GetGlobalMouseState(&_globalMouseDownX, &_globalMouseDownY);
			ControlPresenter::moveTo(_globalMouseDownX - e.downX, _globalMouseDownY - e.downY);
		}
	}
}

bool PWindow::mouseDownInResizeZone(int mouseX, int mouseY) const
{
	return (mouseX < _DRAG_ZONE_DIST || mouseX > ControlPresenter::getWidth() - _DRAG_ZONE_DIST || mouseY < _DRAG_ZONE_DIST || mouseX > ControlPresenter::getHeight() - _DRAG_ZONE_DIST);
}

void PWindow::setResizeCursor(MouseMotionEvent& e) const
{
	if ((e.x < _DRAG_ZONE_DIST) && (e.y < _DRAG_ZONE_DIST))
	{
		SDL_SetCursor(PCursors::SIZENW);
	}
	else if ((e.x > ControlPresenter::getWidth() - _DRAG_ZONE_DIST) && (e.y < _DRAG_ZONE_DIST))
	{
		SDL_SetCursor(PCursors::SIZENE);
	}
	else if ((e.x < _DRAG_ZONE_DIST) && (e.y > ControlPresenter::getHeight() - _DRAG_ZONE_DIST))
	{
		SDL_SetCursor(PCursors::SIZESW);
	}
	else if ((e.x > ControlPresenter::getWidth() - _DRAG_ZONE_DIST) && (e.y > ControlPresenter::getHeight() - _DRAG_ZONE_DIST))
	{
		SDL_SetCursor(PCursors::SIZESE);
	}
	else if (e.y < _DRAG_ZONE_DIST)
	{
		SDL_SetCursor(PCursors::SIZEN);
	}
	else if (e.y > ControlPresenter::getHeight() - _DRAG_ZONE_DIST)
	{
		SDL_SetCursor(PCursors::SIZES);
	}
	else if (e.x < _DRAG_ZONE_DIST)
	{
		SDL_SetCursor(PCursors::SIZEW);
	}
	else if ((e.x > ControlPresenter::getWidth() - _DRAG_ZONE_DIST))
	{
		SDL_SetCursor(PCursors::SIZEE);
	}
	else
	{
		SDL_SetCursor(PCursors::ARROW);
	}
}

void PWindow::dragResize()
{
	int globalMouseX;
	int globalMouseY;
	SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);

	// Check that the mouse has been dragged first; this prevents us from resizing when the mouse is held down but not moving.
	if (globalMouseX != _globalMouseDownX && globalMouseY != _globalMouseDownY)
	{	
		if (SDL_GetCursor() == PCursors::SIZENW)
		{
			ControlPresenter::moveTo(globalMouseX, globalMouseY);
			ControlPresenter::resize(_resizeStartWindowWidth + _globalMouseDownX - globalMouseX, _resizeStartWindowHeight + _globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZENE)
		{
			ControlPresenter::moveTo(ControlPresenter::getX(), globalMouseY);
			ControlPresenter::resize(_resizeStartWindowWidth + globalMouseX - _globalMouseDownX, _resizeStartWindowHeight + _globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZESW)
		{
			ControlPresenter::moveTo(globalMouseX, ControlPresenter::getY());
			ControlPresenter::resize(_resizeStartWindowWidth + _globalMouseDownX - globalMouseX, _resizeStartWindowHeight + globalMouseY - _globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZESE)
		{
			ControlPresenter::resize(_resizeStartWindowWidth + globalMouseX - _globalMouseDownX, _resizeStartWindowHeight + globalMouseY - _globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZEN)
		{
			ControlPresenter::moveTo(ControlPresenter::getX(), globalMouseY);
			ControlPresenter::resize(_resizeStartWindowWidth, _resizeStartWindowHeight + _globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZES)
		{
			ControlPresenter::resize(_resizeStartWindowWidth, _resizeStartWindowHeight + globalMouseY - _globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZEE)
		{
			ControlPresenter::resize(_resizeStartWindowWidth + globalMouseX - _globalMouseDownX, _resizeStartWindowHeight);
		}
		else if (SDL_GetCursor() == PCursors::SIZEW)
		{
			ControlPresenter::moveTo(globalMouseX, ControlPresenter::getY());
			ControlPresenter::resize(_resizeStartWindowWidth + _globalMouseDownX - globalMouseX, _resizeStartWindowHeight);
		}
	}
}
