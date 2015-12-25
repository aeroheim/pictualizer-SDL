#pragma once

#include <string>
#include <SDL.h>

class PWidget;
class Button;
class AudioTrack;
class AudioPlaylist;

class Event
{
	public:
		Event();
		virtual ~Event();
		bool handled;
};

class WindowResizeEvent : public Event
{
	public:
		WindowResizeEvent(int w, int h);
		int ww;
		int wh;
};

class WindowResizedEvent : public Event
{
	public:
		WindowResizedEvent(int w, int h);
		int ww;
		int wh;
};

class FileDropEvent : public Event
{
	public:
		FileDropEvent(SDL_DropEvent& e, int x, int y, bool dir = false);
		std::string path;
		bool dir;
		int x;
		int y;
};

class KeyDownEvent : public Event
{
	public:
		KeyDownEvent(SDL_KeyboardEvent& e);
		SDL_Keycode key;
};

class KeyUpEvent : public Event
{
	public:
		KeyUpEvent(SDL_KeyboardEvent& e);
		SDL_Keycode key;
};

class MouseDownEvent : public Event
{
	public:
		MouseDownEvent(SDL_MouseButtonEvent& e);
		Uint8 button;
		int x;
		int y;
};

class MouseUpEvent : public Event
{
	public:
		MouseUpEvent(SDL_MouseButtonEvent& e);
		Uint8 button;
		int x;
		int y;
};

class MouseWheelEvent : public Event
{
	public:
		MouseWheelEvent(SDL_MouseWheelEvent& e, int mx, int my);
		int mx;
		int my;
		int y;
};

class MouseMotionEvent : public Event
{
	public:
		MouseMotionEvent(SDL_MouseMotionEvent& e);
		int xrel;
		int yrel;
		int x;
		int y;
};

class ImageLoadReadyEvent : public Event
{
	public:
		ImageLoadReadyEvent();
};

class ImageLoadedEvent : public Event
{
	public:
		ImageLoadedEvent(int w, int h);
		int iw;
		int ih;
};

class WidgetMoveEvent : public Event
{
	public:
		WidgetMoveEvent(PWidget* widget, float x, float y);
		PWidget* widget;
		float x;
		float y;
};

class WidgetResizeEvent : public Event
{
	public:
		WidgetResizeEvent(PWidget* widget, float x, float y, float w, float h);
		PWidget* widget;
		float x;
		float y;
		float w;
		float h;
};

class ButtonPressedEvent : public Event
{
	public:
		ButtonPressedEvent(Button* button);
		Button* button;
};

class ButtonToggledEvent : public Event
{
	public:
		ButtonToggledEvent(Button* button, bool on);
		Button* button;
		bool on;
};

class SeekEvent : public Event
{
	public:
		SeekEvent();
};

class SeekRequestEvent : public Event
{
	public:
		SeekRequestEvent(int seconds);
		int seconds;
};

class TrackEnqueuedEvent : public Event
{
	public:
		TrackEnqueuedEvent(int index);
		int index;
};

class TrackRemovedEvent : public Event
{
	public:
		TrackRemovedEvent(int index);
		int index;
};

class PlaylistClearedEvent : public Event
{
	public:
		PlaylistClearedEvent(const AudioPlaylist* playlist);
		const AudioPlaylist* const playlist;
};

class NewTrackEvent : public Event
{
	public:
		NewTrackEvent(const AudioTrack* track);
		const AudioTrack* const track;
};

class PlayerStartedEvent : public Event
{
	public:
		PlayerStartedEvent();
};

class PlayerPausedEvent : public Event
{
	public:
		PlayerPausedEvent();
};

class PlayerStoppedEvent : public Event
{
	public:
		PlayerStoppedEvent();
};

class VolumeChangedEvent : public Event
{
	public:
		VolumeChangedEvent(float vol);
		float vol;
};

