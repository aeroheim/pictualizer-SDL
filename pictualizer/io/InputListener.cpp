#pragma once

#include "InputListener.h"

InputListener::InputListener()
{
	lMouseHeld = false;
	rMouseHeld = false;
}

void InputListener::registerKey(SDL_Keycode key)
{
	keys.insert(std::pair<SDL_Keycode, bool>(key, false));
}

void InputListener::unregisterKey(SDL_Keycode key)
{
	keys.erase(key);
}

bool InputListener::setKeyHeld(SDL_Keycode key)
{
	std::map<SDL_Keycode, bool>::iterator it = keys.find(key);

	if (it != keys.end())
	{
		it->second = true;
		return true;
	}

	return false;
}

bool InputListener::setKeyReleased(SDL_Keycode key)
{
	std::map<SDL_Keycode, bool>::iterator it = keys.find(key);

	if (it != keys.end())
	{
		it->second = false;
		return true;
	}

	return false;
}

bool InputListener::keyHeld(SDL_Keycode key)
{
	std::map<SDL_Keycode, bool>::iterator it = keys.find(key);

	if (it != keys.end())
		return it->second;

	return false;
}