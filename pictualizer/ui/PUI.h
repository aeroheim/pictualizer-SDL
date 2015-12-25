#pragma once

#include <SDL.h>
#include "../controls/AudioPlayerWidget.h"

class PUI : public EventSubscriber, public EventObserver
{
	public:
		PUI(SDL_Renderer* r, AudioPlayer* audioPlayer, int ww, int wh);

		void draw();

		void handleEvent(Event* e);

	private:
		SDL_Renderer* ren;
		int ww;
		int wh;

		AudioPlayer* audioPlayer;
		AudioPlayerWidget audioPlayerWidget;
};