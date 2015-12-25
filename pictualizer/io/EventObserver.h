#pragma once

#include "EventSubscriber.h"
#include <vector>

class EventSubscriber;

class EventObserver
{
	public:
		virtual void addSubscriber(EventSubscriber* s);
		virtual void removeSubscriber(EventSubscriber* s);

	protected:
		std::vector<EventSubscriber*> subscribers;
		virtual void notify(Event* e);
};