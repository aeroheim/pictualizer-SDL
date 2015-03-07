#include "WindowIOController.h"

WindowIOController::WindowIOController(SDL_Window* window)
{
	win = window;

	SDL_GetWindowSize(win, &winWidth, &winHeight);

	dragging = false;
	dragResizing = false;

	PCursors::initCursors();
}

WindowIOController::~WindowIOController()
{
	PCursors::freeCursors();
}

void WindowIOController::pollEvents()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_DROPFILE:
				OnDrop(e.drop);
				break;
			case SDL_KEYDOWN:
				OnKeyDown(e.key);
				break;
			case SDL_KEYUP:
				OnKeyUp(e.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				OnMouseButtonDown(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				OnMouseButtonUp(e.button);
				break;
			case SDL_MOUSEWHEEL:
				OnMouseWheel(e.wheel);
				break;
			// TODO: Mouse cursor is sometimes reset by the OS when outside the SDL window.
			case SDL_MOUSEMOTION:
				OnMouseMotion(e.motion);
				break;
		}
	}
}

void WindowIOController::OnDrop(SDL_DropEvent& e)
{
	FileDropEvent fileDropEvent(e);
	notify(&fileDropEvent);
	SDL_free(e.file);
}

void WindowIOController::OnKeyDown(SDL_KeyboardEvent& e)
{
	KeyDownEvent keyDownEvent(e);
	notify(&keyDownEvent);
}

void WindowIOController::OnKeyUp(SDL_KeyboardEvent& e)
{
	KeyUpEvent keyUpEvent(e);
	notify(&keyUpEvent);
}

void WindowIOController::OnMouseButtonDown(SDL_MouseButtonEvent& e)
{
	MouseDownEvent mouseDownEvent(e);
	notify(&mouseDownEvent);

	if (!mouseDownEvent.handled)
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			// Register global mouse events and deny mouse input to other windows for dragging.
			SDL_CaptureMouse(SDL_TRUE);

			mouseDownX = e.x;
			mouseDownY = e.y;
			SDL_GetGlobalMouseState(&globalMouseDownX, &globalMouseDownY);

			prevWinWidth = winWidth;
			prevWinHeight = winHeight;

			// Window enters drag state when left mouse button is down.
			if (mouseDownInResizeZone())
				dragResizing = true;
			else
				dragging = true;
		}
	}
}

void WindowIOController::OnMouseButtonUp(SDL_MouseButtonEvent& e)
{
	MouseUpEvent mouseUpEvent(e);
	notify(&mouseUpEvent);

	if (!mouseUpEvent.handled)
	{
		if (e.button == SDL_BUTTON_LEFT)
		{
			// Stop registering global mouse events.
			SDL_CaptureMouse(SDL_FALSE);

			dragging = false;
			dragResizing = false;
		}
	}
}

void WindowIOController::OnMouseWheel(SDL_MouseWheelEvent& e)
{
	int mx = 0;
	int my = 0;

	SDL_GetMouseState(&mx, &my);

	MouseWheelEvent mouseWheelEvent(e, mx, my);
	notify(&mouseWheelEvent);
}

void WindowIOController::OnMouseMotion(SDL_MouseMotionEvent& e)
{
	MouseMotionEvent mouseMotionEvent(e);
	notify(&mouseMotionEvent);

	if (!mouseMotionEvent.handled)
	{
		if (dragging)
		{
			SDL_GetGlobalMouseState(&globalMouseDownX, &globalMouseDownY);
			SDL_SetWindowPosition(win, globalMouseDownX - mouseDownX, globalMouseDownY - mouseDownY);
		}
		else if (dragResizing)
		{
			doDragResize();
			OnWindowResized();
		}
		else
			setDragCursor(e);
	}
}

void WindowIOController::OnWindowResized()
{
	SDL_GetWindowSize(win, &winWidth, &winHeight);

	WindowResizeEvent windowResizeEvent(winWidth, winHeight);
	notify(&windowResizeEvent);
}

bool WindowIOController::mouseDownInResizeZone()
{
	if (mouseDownX < DRAG_ZONE_DIST || mouseDownX > winWidth - DRAG_ZONE_DIST ||
		mouseDownY < DRAG_ZONE_DIST || mouseDownY > winHeight - DRAG_ZONE_DIST)
		return true;

	return false;
}

void WindowIOController::setDragCursor(SDL_MouseMotionEvent &motion)
{
	if ((motion.x < DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZENE);
	else if ((motion.x < DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZESE);
	else if (motion.y < DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEN);
	else if (motion.y > winHeight - DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZES);
	else if (motion.x < DRAG_ZONE_DIST)
		SDL_SetCursor(PCursors::SIZEW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST))
		SDL_SetCursor(PCursors::SIZEE);
	else
		SDL_SetCursor(PCursors::ARROW);
}

void WindowIOController::doDragResize()
{
	int globalMouseX, globalMouseY;
	SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);

	// Check that the mouse has been dragged first; this prevents us from resizing when the mouse is held down but not moving.
	if (globalMouseX != globalMouseDownX && globalMouseY != globalMouseDownY)
	{
		int winXPos, winYPos;
		SDL_GetWindowPosition(win, &winXPos, &winYPos);

		if (SDL_GetCursor() == PCursors::SIZENW)
		{
			SDL_SetWindowPosition(win, globalMouseX, globalMouseY);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight + globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZENE)
		{
			SDL_SetWindowPosition(win, winXPos, globalMouseY);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight + globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZESW)
		{
			SDL_SetWindowPosition(win, globalMouseX, winYPos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZESE)
		{
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZEN)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + globalMouseDownY - globalMouseY);
			SDL_SetWindowPosition(win, winXPos, globalMouseY);
		}
		else if (SDL_GetCursor() == PCursors::SIZES)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == PCursors::SIZEE)
		{
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight);
		}
		else if (SDL_GetCursor() == PCursors::SIZEW)
		{
			SDL_SetWindowPosition(win, globalMouseX, winYPos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight);
		}
	}
}
