#pragma once

#include <SDL.h>

namespace PCursors
{
	extern SDL_Cursor* ARROW;
	extern SDL_Cursor* SIZEN;
	extern SDL_Cursor* SIZENW;
	extern SDL_Cursor* SIZENE;
	extern SDL_Cursor* SIZES;
	extern SDL_Cursor* SIZESE;
	extern SDL_Cursor* SIZESW;
	extern SDL_Cursor* SIZEW;
	extern SDL_Cursor* SIZEE;

	void initCursors();
	void freeCursors();
}