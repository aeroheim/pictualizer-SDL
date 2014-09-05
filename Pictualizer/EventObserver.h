#pragma once

#include <vector>
#include <memory>
#include "EventSubscriber.h"

using std::vector;
using std::shared_ptr;

class EventSubscriber;

class EventObserver
{
public:
	virtual void notify(Event e) = 0;

private:
	vector<shared_ptr<EventSubscriber>> subscribers;
};