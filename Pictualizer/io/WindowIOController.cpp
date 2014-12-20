#include "WindowIOController.h"

/*
 *  Insert comment here.
 */
WindowIOController :: WindowIOController(SDL_Window* window)
{
	win = window;

	SDL_GetWindowSize(win, &winWidth, &winHeight);

	dragging = false;
	dragResizing = false;

	// Default arrow cursor
	ARROW = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

	// Cardinal direction cursors used for resizing
	SIZENW = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	SIZESE = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	SIZENE = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	SIZESW = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	SIZEN = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	SIZES = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	SIZEW = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	SIZEE = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
}

/*
 * Insert comment here.
 */
WindowIOController :: ~WindowIOController()
{
	SDL_FreeCursor(ARROW);
	SDL_FreeCursor(SIZEN);
	SDL_FreeCursor(SIZENW);
	SDL_FreeCursor(SIZENE);
	SDL_FreeCursor(SIZES);
	SDL_FreeCursor(SIZESE);
	SDL_FreeCursor(SIZESW);
	SDL_FreeCursor(SIZEW);
	SDL_FreeCursor(SIZEE);
}

/*
 * Insert comment here.
 */
void WindowIOController :: pollEvents()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_DROPFILE:
				OnDrop(e.drop);
				break;
			case SDL_WINDOWEVENT:
				OnWindowResized(e.window);
				break;
			case SDL_MOUSEBUTTONDOWN:
				OnMouseButtonDown(e.button);
				break;
			case SDL_MOUSEBUTTONUP:
				OnMouseButtonUp(e.button);
				break;
			// TODO: Mouse cursor is sometimes reset by the OS when outside the SDL window.
			case SDL_MOUSEMOTION:
				OnMouseMotion(e.motion);
				break;
		}
	}
}

/*
 * Insert comment here.
 */
void WindowIOController :: OnDrop(SDL_DropEvent& e)
{
	// Handle drag & drop here
}

/*
 * Insert comment here.
 */
void WindowIOController :: OnMouseButtonDown(SDL_MouseButtonEvent& e)
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

/*
 * Insert comment here.
 */
void WindowIOController :: OnMouseButtonUp(SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT)
	{
		// Stop registering global mouse events.
		SDL_CaptureMouse(SDL_FALSE);

		dragging = false;
		dragResizing = false;
	}
}

/*
 * Insert comment here.
 */
void WindowIOController :: OnMouseMotion(SDL_MouseMotionEvent& e)
{
	if (dragging)
	{
		SDL_GetGlobalMouseState(&globalMouseDownX, &globalMouseDownY);
		SDL_SetWindowPosition(win, globalMouseDownX - mouseDownX, globalMouseDownY - mouseDownY);
	}
	else if (dragResizing)
	{
		doDragResize();
	}
	else
		setDragCursor(e);
}

/*
 * Insert comment here.
 */
void WindowIOController :: OnWindowResized(SDL_WindowEvent& e)
{
	SDL_GetWindowSize(win, &winWidth, &winHeight);
}

/*
 * Insert comment here.
 */
bool WindowIOController :: mouseDownInResizeZone()
{
	if (mouseDownX < DRAG_ZONE_DIST || mouseDownX > winWidth - DRAG_ZONE_DIST ||
		mouseDownY < DRAG_ZONE_DIST || mouseDownY > winHeight - DRAG_ZONE_DIST)
		return true;

	return false;
}

/*
 * Insert comment here.
 */
void WindowIOController :: setDragCursor(SDL_MouseMotionEvent &motion)
{
	if ((motion.x < DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
		SDL_SetCursor(SIZENW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
		SDL_SetCursor(SIZENE);
	else if ((motion.x < DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
		SDL_SetCursor(SIZESW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
		SDL_SetCursor(SIZESE);
	else if (motion.y < DRAG_ZONE_DIST)
		SDL_SetCursor(SIZEN);
	else if (motion.y > winHeight - DRAG_ZONE_DIST)
		SDL_SetCursor(SIZES);
	else if (motion.x < DRAG_ZONE_DIST)
		SDL_SetCursor(SIZEW);
	else if ((motion.x > winWidth - DRAG_ZONE_DIST))
		SDL_SetCursor(SIZEE);
	else
		SDL_SetCursor(ARROW);
}

/*
 * Insert comment here.
 */
void WindowIOController :: doDragResize()
{
	int globalMouseX, globalMouseY;
	SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);

	// Check that the mouse has been dragged first; this prevents us from resizing when the mouse is held down but not moving.
	if (globalMouseX != globalMouseDownX && globalMouseY != globalMouseDownY)
	{
		int winXPos, winYPos;
		SDL_GetWindowPosition(win, &winXPos, &winYPos);

		if (SDL_GetCursor() == SIZENW)
		{
			SDL_SetWindowPosition(win, globalMouseX, globalMouseY);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight + globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == SIZENE)
		{
			SDL_SetWindowPosition(win, winXPos, globalMouseY);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight + globalMouseDownY - globalMouseY);
		}
		else if (SDL_GetCursor() == SIZESW)
		{
			SDL_SetWindowPosition(win, globalMouseX, winYPos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == SIZESE)
		{
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == SIZEN)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + globalMouseDownY - globalMouseY);
			SDL_SetWindowPosition(win, winXPos, globalMouseY);
		}
		else if (SDL_GetCursor() == SIZES)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + globalMouseY - globalMouseDownY);
		}
		else if (SDL_GetCursor() == SIZEE)
		{
			SDL_SetWindowSize(win, prevWinWidth + globalMouseX - globalMouseDownX, prevWinHeight);
		}
		else if (SDL_GetCursor() == SIZEW)
		{
			SDL_SetWindowPosition(win, globalMouseX, winYPos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - globalMouseX, prevWinHeight);
		}
	}
}
