#pragma once

#include "PCursors.h"

namespace PCursors
{
	SDL_Cursor* ARROW;
	SDL_Cursor* SIZEN;
	SDL_Cursor* SIZENW;
	SDL_Cursor* SIZENE;
	SDL_Cursor* SIZES;
	SDL_Cursor* SIZESE;
	SDL_Cursor* SIZESW;
	SDL_Cursor* SIZEW;
	SDL_Cursor* SIZEE;

	void initCursors()
	{
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

	void freeCursors()
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
}