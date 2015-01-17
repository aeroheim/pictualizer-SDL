#include "Event.h"

Event::~Event() {}

WindowResizeEvent::WindowResizeEvent(int w, int h) : ww(w), wh(h)
{
	handled = false;
}

FileDropEvent::FileDropEvent(SDL_DropEvent& e) : path(e.file)
{
	handled = false;
}

KeyDownEvent::KeyDownEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym)
{
	handled = false;
}

KeyUpEvent::KeyUpEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym)
{
	handled = false;
}

MouseDownEvent::MouseDownEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button)
{
	handled = false;
}

MouseUpEvent::MouseUpEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button)
{
	handled = false;
}

MouseWheelEvent::MouseWheelEvent(SDL_MouseWheelEvent& e, int mx, int my) : y(e.y), mx(mx), my(my)
{
	handled = false;
}

MouseMotionEvent::MouseMotionEvent(SDL_MouseMotionEvent& e) : xrel(e.xrel), yrel(e.yrel), x(e.x), y(e.y)
{
	handled = false;
}
