#pragma once

#include <SDL.h>
#include "../util/utils.h"
#include "../controls/Label.h"
#include "../controls/GridPanel.h"

class PUI
{
	public:
		PUI(SDL_Window* win, SDL_Renderer* r, int ww, int wh);

		void draw();

	private:
		SDL_Window* win;
		SDL_Renderer* ren;
		int ww;
		int wh;

		GridPanel test;

		TTF_Font* hans;
		TTF_Font* mplus;
};