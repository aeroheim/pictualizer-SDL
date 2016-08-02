#pragma once

#include "EventObserver.h"
#include <unordered_set>

class EventManager : public EventObserver
{
	public:
		static EventManager& getInstance();
		void pollEvents();
		bool isKeyDown(SDL_Keycode key);

	private:
		static EventManager _eventManager;
		SDL_Event _event;

		std::unordered_set<SDL_Keycode> _keysHeld;
		int _mouseX = 0;
		int _mouseY = 0;
		int _mouseDownX = 0;
		int _mouseDownY = 0;
		bool _isLeftMouseButtonDown = false;

		EventManager() = default;
		EventManager(const EventManager& other) = delete;
		EventManager& operator=(const EventManager& other) = delete;
		~EventManager();
		void OnDrop(SDL_DropEvent& e);
		void OnKeyDown(SDL_KeyboardEvent& e);
		void OnKeyUp(SDL_KeyboardEvent& e);
		void OnMouseButtonDown(SDL_MouseButtonEvent& e);
		void OnMouseButtonUp(SDL_MouseButtonEvent& e);
		void OnMouseWheel(SDL_MouseWheelEvent& e);
		void OnMouseMotion(SDL_MouseMotionEvent& e);
		void OnMouseDrag(SDL_MouseMotionEvent& e);
};
