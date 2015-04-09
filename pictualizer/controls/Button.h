#pragma once

#include <SDL.h>
#include "PControl.h"
#include "ButtonStates.h"
#include "../io/InputListener.h"
#include "../io/EventObserver.h"

class Button : public PControl, public EventSubscriber, public EventObserver, public InputListener
{
	public:
		bool isToggled();
		void setButtonStyle(ButtonStyle s);
		ButtonStyle getButtonStyle();
		virtual void handleEvent(Event* e);
	
	protected:
		Button(float x, float y, float w, float h);
		~Button();

		void OnMouseUp();

	private:
		ButtonStyle style;
		bool toggled;
};