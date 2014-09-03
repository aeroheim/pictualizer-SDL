#include <vector>
#include <memory>
#include "EventSubscriber.h"

class EventObserver
{
public:
	template<typename Data>
	virtual void notify(Data data);

private:
	std::vector<std::shared_ptr<EventSubscriber>> subscribers;
};