#include "EventObserver.h"

void EventObserver::addSubscriber(EventSubscriber* s)
{
	subscribers.push_back(s);
}

void EventObserver::removeSubscriber(EventSubscriber* s)
{
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), s), subscribers.end());
}

void EventObserver::notify(Event e)
{
	for (EventSubscriber* s : subscribers)
		s->handleEvent(e);
}