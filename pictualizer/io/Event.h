#pragma once

#include <SDL.h>
#include <string>

class PWidget;
class Button;

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

class FileDropEvent : public Event
{
	public:
		FileDropEvent(SDL_DropEvent& e, int x, int y);
		std::string path;
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
		Button *button;
		bool on;
};

class TrackSeekEvent : public Event
{
	public:
		TrackSeekEvent(int seconds);
		int seconds;
};

