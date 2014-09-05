#pragma once

#include "EventObserver.h"
#include <SDL.h>
#include <memory>
#include <Windows.h>

using std::shared_ptr;

class WindowIOController : public EventObserver
{
public:
	WindowIOController(SDL_Window* win);
	~WindowIOController();
	void pollEvents();
	void notify(Event e);
private:
	const int DRAG_ZONE_DIST = 15;

	SDL_Window* win;
	SDL_Event e;

	int mouseDownX, mouseDownY;
	int globalMouseDownX, globalMouseDownY;
	bool dragResizing;

	int prevWinWidth, prevWinHeight;
	int winWidth, winHeight;

	SDL_Cursor* ARROW;
	SDL_Cursor* SIZEN;
	SDL_Cursor* SIZENW;
	SDL_Cursor* SIZENE;
	SDL_Cursor* SIZES;
	SDL_Cursor* SIZESE;
	SDL_Cursor* SIZESW;
	SDL_Cursor* SIZEW;
	SDL_Cursor* SIZEE;

	void setDragCursor(SDL_MouseMotionEvent& e); 
	void doDragResize(POINT& newpt);

	bool mouseInResizeZone();
};