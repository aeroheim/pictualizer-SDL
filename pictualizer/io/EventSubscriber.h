#pragma once

#include "Event.h"
#include "EventObserver.h"

class EventObserver;

class EventSubscriber
{
	public:
		virtual void handleEvent(Event* e) = 0;

	protected:
		virtual void subscribeTo(EventObserver* o);
		virtual void unsubscribeFrom(EventObserver* o);
};