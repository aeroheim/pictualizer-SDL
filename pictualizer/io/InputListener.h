#pragma once

#include <SDL.h>
#include <map>

class InputListener
{
	public:
		InputListener();

	protected:
		std::map<SDL_Keycode, bool> keys;
		void registerKey(SDL_Keycode key);
		void unregisterKey(SDL_Keycode key);

		bool setKeyHeld(SDL_Keycode key);
		bool setKeyReleased(SDL_Keycode key);
		bool keyHeld(SDL_Keycode key);

		bool lMouseHeld;
		bool rMouseHeld;
		int mDownX;
		int mDownY;
};