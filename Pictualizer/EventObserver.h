#pragma once

#include "EventSubscriber.h"
#include <algorithm>
#include <vector>

using std::vector;

class EventSubscriber;

class EventObserver
{
	public:
		virtual void addSubscriber(EventSubscriber* s);
		virtual void removeSubscriber(EventSubscriber* s);

	private:
		vector<EventSubscriber*> subscribers;
		virtual void notify(Event e);
};