#pragma once

#include <SDL.h>
#include <string>

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
		FileDropEvent(SDL_DropEvent& e);
		std::string path;
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

