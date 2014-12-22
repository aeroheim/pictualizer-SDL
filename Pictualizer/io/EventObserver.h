#pragma once

#include "EventSubscriber.h"
#include <algorithm>
#include <vector>

class EventSubscriber;

class EventObserver
{
	public:
		virtual void addSubscriber(EventSubscriber* s);
		virtual void removeSubscriber(EventSubscriber* s);

	private:
		std::vector<EventSubscriber*> subscribers;
		virtual void notify(Event e);
};