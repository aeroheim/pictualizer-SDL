#pragma once

#include "Event.h"

Event::Event() : handled(false) {}

Event::~Event() {}

WindowResizedEvent::WindowResizedEvent(int w, int h) : ww(w), wh(h) {}

FileDropEvent::FileDropEvent(SDL_DropEvent& e, int x, int y, bool dir) : path(e.file), dir(dir), x(x), y(y) {}

KeyDownEvent::KeyDownEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

KeyUpEvent::KeyUpEvent(SDL_KeyboardEvent& e) : key(e.keysym.sym) {}

MouseDownEvent::MouseDownEvent(SDL_MouseButtonEvent& e) : button(e.button), x(e.x), y(e.y) {}

MouseUpEvent::MouseUpEvent(SDL_MouseButtonEvent& e) : button(e.button), x(e.x), y(e.y) {}

MouseWheelEvent::MouseWheelEvent(SDL_MouseWheelEvent& e, int mx, int my) : x(x), y(y), scrollY(e.y) {}

MouseMotionEvent::MouseMotionEvent(SDL_MouseMotionEvent& e) : x(e.x), y(e.y), xrel(e.xrel), yrel(e.yrel) {}

MouseDragEvent::MouseDragEvent(SDL_MouseMotionEvent& e, int downX, int downY) : x(e.x), y(e.y), xrel(e.xrel), yrel(e.yrel), downX(downX), downY(downY) {}

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