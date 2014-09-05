#pragma once

#include "Event.h"

class EventSubscriber
{
	void handleEvent(Event event);
	void subscribeTo(EventObserver o);
};