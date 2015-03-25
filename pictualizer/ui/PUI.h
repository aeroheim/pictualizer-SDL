#pragma once

#include <SDL.h>
#include "../util/PUtils.h"
#include "../controls/Label.h"
#include "../controls/AudioPlayerWidget.h"
#include "../controls/GridPanel.h"

class PUI : public EventSubscriber, public EventObserver
{
	public:
		PUI(SDL_Window* win, SDL_Renderer* r, int ww, int wh);

		void draw();

		void handleEvent(Event* e);

	private:
		SDL_Window* win;
		SDL_Renderer* ren;
		int ww;
		int wh;

		AudioPlayerWidget audioPlayerWidget;

		TTF_Font* gothic;
		TTF_Font* mplus;
};