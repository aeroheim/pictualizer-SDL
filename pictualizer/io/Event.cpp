#include "Event.h"

Event::Event() : handled(false) {}

Event::~Event() {}

WindowResizeEvent::WindowResizeEvent(int w, int h) : ww(w), wh(h) {}

FileDropEvent::FileDropEvent(SDL_DropEvent& e) : path(e.file) {}

KeyDownEvent::KeyDownEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

KeyUpEvent::KeyUpEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

MouseDownEvent::MouseDownEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button) {}

MouseUpEvent::MouseUpEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button) {}

MouseWheelEvent::MouseWheelEvent(SDL_MouseWheelEvent& e, int mx, int my) : y(e.y), mx(mx), my(my) {}

MouseMotionEvent::MouseMotionEvent(SDL_MouseMotionEvent& e) : xrel(e.xrel), yrel(e.yrel), x(e.x), y(e.y) {}

ImageLoadReadyEvent::ImageLoadReadyEvent() {}

ImageLoadedEvent::ImageLoadedEvent(int w, int h) : iw(w), ih(h) {}
