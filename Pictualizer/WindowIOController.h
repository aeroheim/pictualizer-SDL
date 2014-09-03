#pragma once

#include "EventObserver.h"
#include <SDL.h>
#include <memory>

using std::shared_ptr;

class WindowIOController : EventObserver
{
public:
	void pollEvents();
private:
	shared_ptr<SDL_Window> win;
};