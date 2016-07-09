#pragma once

#include "EventSubscriber.h"

EventSubscriber::~EventSubscriber()
{
}

void EventSubscriber::subscribeTo(EventObserver* o)
{
	o->addSubscriber(this);
}

void EventSubscriber::unsubscribeFrom(EventObserver* o)
{
	o->removeSubscriber(this);
}
