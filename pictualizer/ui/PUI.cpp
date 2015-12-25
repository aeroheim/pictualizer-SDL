#pragma once

#include "PUI.h"

PUI::PUI(SDL_Renderer* ren, AudioPlayer* audioPlayer, int ww, int hh) : 
	ren(ren), ww(ww), wh(hh), 
	audioPlayer(audioPlayer),
	audioPlayerWidget(ren, audioPlayer, ww / 6.0f, wh / 3.0f, ww * (2.0f / 3.0f), wh * (2.0f / 5.0f))
{
	addSubscriber(&audioPlayerWidget);
}

void PUI::draw()
{
	audioPlayerWidget.draw();
}

void PUI::handleEvent(Event* e)
{
	notify(e);
}