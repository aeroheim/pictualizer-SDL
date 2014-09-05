#include <vector>
#include <memory>
#include "EventSubscriber.h"

class EventObserver
{
public:
	virtual void notify(Event e);

private:
	std::vector<std::shared_ptr<EventSubscriber>> subscribers;
};