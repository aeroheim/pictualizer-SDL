#pragma once

#include "EventObserver.h"
#include "../controls/PWindow.h"
#include <unordered_map>

class WindowManager : public EventObserver, public EventSubscriber
{

	// probably an unordered_map<windowID, PWindow*>
	// constructor/destructor of PWindow needs to register/unregister with WindowManager
	
	// WindowManager event handler needs to forward the events to the correct 

	public:
		static WindowManager& getInstance();
		void addWindow(PWindow* window);
		void removeWindow(PWindow* window);

		void handleEvent(Event* e) override;

	private:
		static WindowManager _windowManager;
		std::unordered_map<unsigned int, PWindow*> _windows;
}
