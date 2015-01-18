#pragma once

#include "../audio/AudioTrack.h"
#include "EventObserver.h"
#include "iostream"
#include <SDL.h>
#pragma once

#include <locale>
#include <codecvt>

class WindowIOController : public EventObserver
{
	public:
		WindowIOController(SDL_Window* win);
		~WindowIOController();

		void pollEvents();

	private:
		const int DRAG_ZONE_DIST = 15;
		SDL_Window* win;
		SDL_Event e;
		SDL_Cursor* ARROW;
		SDL_Cursor* SIZEN;
		SDL_Cursor* SIZENW;
		SDL_Cursor* SIZENE;
		SDL_Cursor* SIZES;
		SDL_Cursor* SIZESE;
		SDL_Cursor* SIZESW;
		SDL_Cursor* SIZEW;
		SDL_Cursor* SIZEE;

		void OnDrop(SDL_DropEvent& e);
		void OnKeyDown(SDL_KeyboardEvent& e);
		void OnKeyUp(SDL_KeyboardEvent& e);
		void OnMouseButtonDown(SDL_MouseButtonEvent& e);
		void OnMouseButtonUp(SDL_MouseButtonEvent& e);
		void OnMouseWheel(SDL_MouseWheelEvent& e);
		void OnMouseMotion(SDL_MouseMotionEvent& e);
		void OnWindowResized();

		int mouseDownX, mouseDownY;
		int globalMouseDownX, globalMouseDownY;
		int winWidth, winHeight;
		int prevWinWidth, prevWinHeight;

		bool dragging;
		bool dragResizing;
		bool mouseDownInResizeZone();

		void setDragCursor(SDL_MouseMotionEvent& e);
		void doDragResize();
};