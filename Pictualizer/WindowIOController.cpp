#include "WindowIOController.h"

void WindowIOController::pollEvents()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_DROPFILE:
			// TODO: Interface with Background;
			//std::string imagePath = e.drop.file;
			break;
		case SDL_WINDOWEVENT:
			SDL_GetWindowSize(win, &winWidth, &winHeight);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				mouseDownX = e.button.x;
				mouseDownY = e.button.y;
				POINT pt;
				GetCursorPos(&pt);
				globalMouseDownX = pt.x;
				globalMouseDownY = pt.y;

				prevWinWidth = winWidth;
				prevWinHeight = winHeight;

				dragResizing = mouseInResizeZone();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			dragResizing = false;
			break;
		case SDL_MOUSEMOTION:
			if (!dragResizing)
				setDragCursor(e.motion);
			break;

		}
	}
	if ((GetKeyState(VK_LBUTTON) & 0x80) != 0)
	{
		
		int wx, wy;
		POINT currPos;
		GetCursorPos(&currPos);
		if (dragResizing)
			doDragResize(currPos);
		else 
		{
			SDL_GetWindowPosition(win, &wx, &wy);
			SDL_SetWindowPosition(win, currPos.x - mouseDownX, currPos.y - mouseDownY);
		}
		
		
	}
}

bool WindowIOController::mouseInResizeZone()
{
	if (mouseDownX < DRAG_ZONE_DIST || mouseDownX > winWidth - DRAG_ZONE_DIST ||
		mouseDownY < DRAG_ZONE_DIST || mouseDownY > winHeight - DRAG_ZONE_DIST)
		return true;
	return false;
}

void WindowIOController::notify(Event e)
{

}

void WindowIOController::doDragResize(POINT& newpt)
{
	// Check that the mouse has been dragged first
	if (newpt.x != globalMouseDownX && newpt.y != globalMouseDownY)
	{
		int wxpos, wypos;
		SDL_GetWindowPosition(win, &wxpos, &wypos);

		// NW drag resizing
		if (SDL_GetCursor() == SIZENW)
		{
			SDL_SetWindowPosition(win, newpt.x, newpt.y);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - newpt.x, prevWinHeight + globalMouseDownY - newpt.y);
		}
		// NE drag resizing
		else if (SDL_GetCursor() == SIZENE)
		{
			SDL_SetWindowPosition(win, wxpos, newpt.y);
			SDL_SetWindowSize(win, prevWinWidth + newpt.x - globalMouseDownX, prevWinHeight + globalMouseDownY - newpt.y);
		}
		// SW drag resizing
		else if (SDL_GetCursor() == SIZESW)
		{
			SDL_SetWindowPosition(win, newpt.x, wypos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - newpt.x, prevWinHeight + newpt.y - globalMouseDownY);
		}
		// SE drag resizing
		else if (SDL_GetCursor() == SIZESE)
		{
			SDL_SetWindowSize(win, prevWinWidth + newpt.x - globalMouseDownX, prevWinHeight + newpt.y - globalMouseDownY);
		}
		// N drag resizing
		else if (SDL_GetCursor() == SIZEN)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + globalMouseDownY - newpt.y);
			SDL_SetWindowPosition(win, wxpos, newpt.y);
		}
		// S drag resizing
		else if (SDL_GetCursor() == SIZES)
		{
			SDL_SetWindowSize(win, prevWinWidth, prevWinHeight + newpt.y - globalMouseDownY);
		}
		// E drag resizing
		else if (SDL_GetCursor() == SIZEE)
		{
			SDL_SetWindowSize(win, prevWinWidth + newpt.x - globalMouseDownX, prevWinHeight);
		}
		// W drag resizing
		else if (SDL_GetCursor() == SIZEW)
		{
			SDL_SetWindowPosition(win, newpt.x, wypos);
			SDL_SetWindowSize(win, prevWinWidth + globalMouseDownX - newpt.x, prevWinHeight);
		}
	}
}

void WindowIOController::setDragCursor(SDL_MouseMotionEvent &motion)
{
	// NW drag resize cursor
	if ((motion.x < DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
	{
		SDL_SetCursor(SIZENW);
	}
	// NE drag resize cursor
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y < DRAG_ZONE_DIST))
	{
		SDL_SetCursor(SIZENE);
	}
	// SW drag resize cursor
	else if ((motion.x < DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
	{
		SDL_SetCursor(SIZESW);
	}
	// SE drag resize cursor
	else if ((motion.x > winWidth - DRAG_ZONE_DIST) && (motion.y > winHeight - DRAG_ZONE_DIST))
	{
		SDL_SetCursor(SIZESE);
	}
	// N drag resize cursor
	else if (motion.y < DRAG_ZONE_DIST)
	{
		SDL_SetCursor(SIZEN);
	}
	// S drag resize cursor
	else if (motion.y > winHeight - DRAG_ZONE_DIST)
	{
		SDL_SetCursor(SIZES);
	}
	// W drag resize cursor
	else if (motion.x < DRAG_ZONE_DIST)
	{
		SDL_SetCursor(SIZEW);
	}
	// E drag resize cursor
	else if ((motion.x > winWidth - DRAG_ZONE_DIST))
	{
		SDL_SetCursor(SIZEE);
	}
	// No drag resize cursor
	else
	{
		SDL_SetCursor(ARROW);
	}
}

WindowIOController::WindowIOController(SDL_Window* window)
{
	win = window;

	SDL_GetWindowSize(win, &winWidth, &winHeight);

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

WindowIOController::~WindowIOController()
{
	free(ARROW);
	free(SIZEN);
	free(SIZENW);
	free(SIZENE);
	free(SIZES);
	free(SIZESE);
	free(SIZESW);
	free(SIZEW);
	free(SIZEE);
}