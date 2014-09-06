#include "EventSubscriber.h"

void EventSubscriber :: subscribeTo(EventObserver* o)
{
	o->addSubscriber(this);
}

void EventSubscriber :: unsubscribeFrom(EventObserver* o)
{
	o->removeSubscriber(this);
}