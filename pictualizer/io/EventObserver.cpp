#pragma once

#include "EventObserver.h"
#include <algorithm>

void EventObserver::addSubscriber(EventSubscriber* s)
{
	subscribers.push_back(s);
}

void EventObserver::removeSubscriber(EventSubscriber* s)
{
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), s), subscribers.end());
}

EventObserver::~EventObserver()
{
}

void EventObserver::notify(Event* e)
{
	for (EventSubscriber* s : subscribers)
	{
		if (!e->handled)
		{
			s->handleEvent(e);
		}
		else
		{
			break;
		}
	}
}
