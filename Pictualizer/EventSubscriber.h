#pragma once

#include "Event.h"
#include "EventObserver.h"
#include <memory>

using std::shared_ptr;

class EventObserver;

class EventSubscriber
{
public:
	virtual void handleEvent(Event event) = 0;
	virtual void subscribeTo(shared_ptr<EventObserver> o) = 0;
};