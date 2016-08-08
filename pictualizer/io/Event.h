#pragma once

#include <string>
#include <SDL.h>

class PWidget;
class Button;
class AudioTrack;
class AudioPlaylist;

// Abstract base class for all events.
class Event
{
	public:
		virtual ~Event();
		bool handled;

	protected:
		Event();
};

// Abstract base class for events that belong to a specific window.
class WindowEvent : public Event
{
	public:
		virtual ~WindowEvent();
		const int windowID;

	protected:
		explicit WindowEvent(int windowID);
};

struct WindowResizedEvent : Event
{
	WindowResizedEvent(int w, int h);
	const int ww;
	const int wh;
};

struct FileDropEvent : Event
{
	FileDropEvent(SDL_DropEvent& e, int x, int y, bool dir = false);
	const std::string path;
	const bool dir;
	const int x;
	const int y;
};

struct KeyDownEvent : WindowEvent
{
	explicit KeyDownEvent(SDL_KeyboardEvent& e);
	const SDL_Keycode key;
};

struct KeyUpEvent : WindowEvent
{
	explicit KeyUpEvent(SDL_KeyboardEvent& e);
	const SDL_Keycode key;
};

struct MouseDownEvent : WindowEvent
{
	explicit MouseDownEvent(SDL_MouseButtonEvent& e);
	const Uint8 button;
	const int x;
	const int y;
};

struct MouseUpEvent : WindowEvent
{
	explicit MouseUpEvent(SDL_MouseButtonEvent& e);
	const Uint8 button;
	const int x;
	const int y;
};

struct MouseWheelEvent : WindowEvent
{
	MouseWheelEvent(SDL_MouseWheelEvent& e, int x, int y);
	const int x;
	const int y;
	const int scrollY;
};

struct MouseMotionEvent : WindowEvent
{
	explicit MouseMotionEvent(SDL_MouseMotionEvent& e);
	const int x;
	const int y;
	const int xrel;
	const int yrel;
};

struct MouseDragEvent : WindowEvent
{
	MouseDragEvent(SDL_MouseMotionEvent& e, int downX, int downY);
	const int x;
	const int y;
	const int xrel;
	const int yrel;
	const int downX;
	const int downY;
};

struct ImageLoadReadyEvent : Event
{
	ImageLoadReadyEvent();
};

struct ImageLoadedEvent : Event
{
	ImageLoadedEvent(int w, int h);
	const int iw;
	const int ih;
};

struct WidgetMoveEvent : Event
{
	WidgetMoveEvent(PWidget* widget, float x, float y);
	const PWidget* const widget;
	const float x;
	const float y;
};

struct WidgetResizeEvent : Event
{
	WidgetResizeEvent(PWidget* widget, float x, float y, float w, float h);
	const PWidget* const widget;
	const float x;
	const float y;
	const float w;
	const float h;
};

struct ButtonPressedEvent : Event
{
	explicit ButtonPressedEvent(Button* button);
	const Button* const button;
};

struct ButtonToggledEvent : Event
{
	ButtonToggledEvent(Button* button, bool on);
	const Button* const button;
	const bool on;
};

struct SeekEvent : Event
{
	SeekEvent();
};

struct SeekRequestEvent : Event
{
	explicit SeekRequestEvent(int seconds);
	const int seconds;
};

struct TrackEnqueuedEvent : Event
{
	explicit TrackEnqueuedEvent(int index);
	const int index;
};

struct TrackRemovedEvent : Event
{
	explicit TrackRemovedEvent(int index);
	const int index;
};

struct PlaylistClearedEvent : Event
{
	explicit PlaylistClearedEvent(const AudioPlaylist* playlist);
	const AudioPlaylist* const playlist;
};

struct NewTrackEvent : Event
{
	explicit NewTrackEvent(const AudioTrack* track);
	const AudioTrack* const track;
};

struct  PlayerStartedEvent : Event
{
	PlayerStartedEvent();
};

struct PlayerPausedEvent : Event
{
	PlayerPausedEvent();
};

struct PlayerStoppedEvent : Event
{
	PlayerStoppedEvent();
};

struct VolumeChangedEvent : Event
{
	explicit VolumeChangedEvent(float vol);
	const float vol;
};

