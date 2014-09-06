#pragma once

enum class EventID
{
	// TODO: Find a better categorization 
	AUDIO_STOPPED,
	AUDIO_STARTED,
};

class Event 
{
	public:
		Event(EventID id);
		EventID id;
		bool handled;
};