#include "Event.h"

Event::Event() : handled(false) {}

Event::~Event() {}

WindowResizeEvent::WindowResizeEvent(int w, int h) : ww(w), wh(h) {}

FileDropEvent::FileDropEvent(SDL_DropEvent& e, int x, int y, bool dir) : path(e.file), dir(dir), x(x), y(y) {}

KeyDownEvent::KeyDownEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

KeyUpEvent::KeyUpEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

MouseDownEvent::MouseDownEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button) {}

MouseUpEvent::MouseUpEvent(SDL_MouseButtonEvent& e) : x(e.x), y(e.y), button(e.button) {}

MouseWheelEvent::MouseWheelEvent(SDL_MouseWheelEvent& e, int mx, int my) : y(e.y), mx(mx), my(my) {}

MouseMotionEvent::MouseMotionEvent(SDL_MouseMotionEvent& e) : xrel(e.xrel), yrel(e.yrel), x(e.x), y(e.y) {}

ImageLoadReadyEvent::ImageLoadReadyEvent() {}

ImageLoadedEvent::ImageLoadedEvent(int w, int h) : iw(w), ih(h) {}

WidgetMoveEvent::WidgetMoveEvent(PWidget* widget, float x, float y) : widget(widget), x(x), y(y) {}

WidgetResizeEvent::WidgetResizeEvent(PWidget* widget, float x, float y, float w, float h) : widget(widget), x(x), y(y), w(w), h(h) {}

ButtonPressedEvent::ButtonPressedEvent(Button* button) : button(button) {}

ButtonToggledEvent::ButtonToggledEvent(Button* button, bool on) : button(button), on(on) {}

SeekEvent::SeekEvent() {}

SeekRequestEvent::SeekRequestEvent(int seconds) : seconds(seconds) {}

TrackEnqueuedEvent::TrackEnqueuedEvent(int index) : index(index) {}

TrackRemovedEvent::TrackRemovedEvent(int index) : index(index) {}

PlaylistClearedEvent::PlaylistClearedEvent(const AudioPlaylist* playlist) : playlist(playlist) {}

NewTrackEvent::NewTrackEvent(const AudioTrack* track) : track(track) {}

PlayerStartedEvent::PlayerStartedEvent() {}

PlayerPausedEvent::PlayerPausedEvent() {}

PlayerStoppedEvent::PlayerStoppedEvent() {}

VolumeChangedEvent::VolumeChangedEvent(float vol) : vol(vol) {}