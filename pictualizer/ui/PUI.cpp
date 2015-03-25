#include "PUI.h"
#include <iostream>

using namespace std;

PUI::PUI(SDL_Window* win, SDL_Renderer* ren, int ww, int hh) : 
	win(win), 
	ren(ren), ww(ww), wh(hh), 
	audioPlayerWidget(ren, ww / 6.0f, wh / 3.0f, ww * (2.0f / 3.0f), wh * (2.0f / 5.0f))
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