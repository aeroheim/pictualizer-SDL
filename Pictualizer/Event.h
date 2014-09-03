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
	EventID id;
};