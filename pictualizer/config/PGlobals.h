#pragma once

#include <SDL.h>

namespace PGlobals
{
	namespace PConstants
	{
		const int P_FRAMERATE = 60;
		const Uint8 P_MAX_ALPHA = SDL_ALPHA_OPAQUE;
		const Uint8 P_MIN_ALPHA = SDL_ALPHA_TRANSPARENT;
		const Uint8 P_MAX_RGB = 255;
		const Uint8 P_MIN_RGB = 0;

		const Uint8 PCONTROL_MAX_ALPHA = P_MAX_ALPHA;
		const Uint8 PCONTROL_MIN_ALPHA = P_MIN_ALPHA;
		const Uint8 PCONTROL_MAX_RGB = P_MAX_RGB;
		const Uint8 PCONTROL_MIN_RGB = P_MIN_RGB;
		const float PCONTROL_DEFAULT_MAXDIM = 4320;

		const float PWIDGET_DRAG_ZONE_DIST = 12;
		const float PWIDGET_SMOOTH_RESIZE_SCALE = 0.50f;

		const float IMGBG_PAN_SPEED = 0.2f;
		const float IMGBG_MAX_ALPHA = 50;
		const float IMGBG_MIN_ALPHA = P_MIN_ALPHA;
		const float IMGBG_ROAM_FADE_SPEED = 0.5f;
		const float IMGBG_MANUAL_FADE_SPEED = 0.5f;
	}

	namespace PVariables
	{
		static SDL_Keycode IMGBG_ACCESS_KEY = SDLK_LCTRL;
		static SDL_Keycode IMGBG_PREV_IMG_KEY = SDLK_LEFT;
		static SDL_Keycode IMGBG_NEXT_IMG_KEY = SDLK_RIGHT;
	}
}