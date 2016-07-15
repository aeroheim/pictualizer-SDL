#pragma once

#include "EventManager.h"
#include "../util/PUtils.h"

EventManager& EventManager::getInstance()
{
	return _eventManager;
}

void EventManager::pollEvents()
{
	while (SDL_PollEvent(&_event) != 0)
	{
		switch (_event.type)
		{
			case SDL_DROPFILE:
				OnDrop(_event.drop);
				break;
			case SDL_KEYDOWN:
				OnKeyDown(_event.key);
				break;
			case SDL_KEYUP:
				OnKeyUp(_event.key);
				break;
			case SDL_MOUSEBUTTONDOWN:
				OnMouseButtonDown(_event.button);
				break;
			case SDL_MOUSEBUTTONUP:
				OnMouseButtonUp(_event.button);
				break;
			case SDL_MOUSEWHEEL:
				OnMouseWheel(_event.wheel);
				break;
			case SDL_MOUSEMOTION:
				if (_isLeftMouseButtonDown)
				{
					OnMouseDrag(_event.motion);
				}
				else
				{
					OnMouseMotion(_event.motion);
				}
				break;
		}
	}
}

bool EventManager::isKeyDown(SDL_Keycode key)
{
	return _keysHeld.find(key) != _keysHeld.end();
}

EventManager::~EventManager()
{
}

void EventManager::OnDrop(SDL_DropEvent& e)
{
	FileDropEvent fileDropEvent(e, _mouseX, _mouseY, PUtils::pathIsDirectory(PUtils::str2wstr(e.file)));
	notify(&fileDropEvent);
	SDL_free(e.file);
}

void EventManager::OnKeyDown(SDL_KeyboardEvent& e)
{
	_keysHeld.insert(e.keysym.sym);

	KeyDownEvent keyDownEvent(e);
	notify(&keyDownEvent);
}

void EventManager::OnKeyUp(SDL_KeyboardEvent& e)
{
	_keysHeld.erase(e.keysym.sym);

	KeyUpEvent keyUpEvent(e);
	notify(&keyUpEvent);
}

void EventManager::OnMouseButtonDown(SDL_MouseButtonEvent& e)
{
	_mouseX = e.x;
	_mouseY = e.y;
	_isLeftMouseButtonDown = e.button == SDL_BUTTON_LEFT; 
	_mouseDownX = e.x;
	_mouseDownY = e.y;

	MouseDownEvent mouseDownEvent(e);
	notify(&mouseDownEvent);
}

void EventManager::OnMouseButtonUp(SDL_MouseButtonEvent& e)
{
	_mouseX = e.x;
	_mouseY = e.y;
	_isLeftMouseButtonDown = e.button == SDL_BUTTON_LEFT;

	MouseUpEvent mouseUpEvent(e);
	notify(&mouseUpEvent);
}

void EventManager::OnMouseWheel(SDL_MouseWheelEvent& e)
{
	SDL_GetMouseState(&_mouseX, &_mouseY);

	MouseWheelEvent mouseWheelEvent(e, _mouseX, _mouseY);
	notify(&mouseWheelEvent);
}

void EventManager::OnMouseMotion(SDL_MouseMotionEvent& e)
{
	_mouseX = e.x;
	_mouseY = e.y;

	MouseMotionEvent mouseMotionEvent(e);
	notify(&mouseMotionEvent);
}

void EventManager::OnMouseDrag(SDL_MouseMotionEvent& e)
{
	_mouseX = e.x;
	_mouseY = e.y;

	MouseDragEvent mouseDragEvent(e, _mouseDownX, _mouseDownY);
	notify(&mouseDragEvent);
}