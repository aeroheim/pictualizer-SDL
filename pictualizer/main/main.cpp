#pragma once

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>
#include <iostream>
#include "../io/WindowIOController.h"
#include "../image/ImageBackground.h"
#include "../assets/textures/PTextures.h"
#include "../assets/fonts/PFonts.h"
#include "../ui/PUI.h"

using std::cout;
using std::endl;

bool initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL Initialization Error: " << SDL_GetError() << endl;
		return false;
	}

	// Create SDL window.
	// NOTE: Window size should eventually be saved and loaded from a preferences file.
	window = SDL_CreateWindow("Pictualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 530, SDL_WINDOW_BORDERLESS);
	if (window == nullptr)
	{
		cout << "Window Initialization Error: " << SDL_GetError << endl;
		return 1;
	}

	// Create SDL renderer.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (renderer == nullptr)
	{
		cout << "Renderer Initialization Error: " << SDL_GetError() << endl;
		return 1;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	// Use blending to allow for alpha effects on textures.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");		// Use linear filtering to preserve image quality when scaled.

	// Initialize SDL_TTF.
	if (TTF_Init() < 0)
	{
		cout << "Font Initialization Error: " << TTF_GetError() << endl;
		return 1;
	}

	return true;
}

bool initBASS(SDL_Window* window)
{
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	SDL_GetWindowWMInfo(window, &windowInfo);

	// Initialize BASS.
	if (!BASS_Init(-1, 44100, 0, windowInfo.info.win.window, nullptr))
	{
		cout << "BASS Initialization Error: " << BASS_ErrorGetCode() << endl;
		return false;
	}

	return true;
}

int main(int argc, char** argv)
{
	SDL_Window* window;
	SDL_Renderer* renderer;

	if (!initSDL(window, renderer) || !initBASS(window))
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}

	int ww, wh;
	SDL_GetWindowSize(window, &ww, &wh);

	// Initialize fonts.
	PFonts::initFonts();

	// Initialize textures.
	PTextures::initTextures(renderer);

	// Initialize image background.
	ImageBackground imageBackground(renderer, ww, wh);
	
	// Initialize audio player.
	AudioPlayer audioPlayer;
	audioPlayer.setVolume(0.5f);

	// Initialize UI.
	PUI ui(renderer, &audioPlayer, ww, wh);

	// Initialize IO.
	WindowIOController windowIOController(window);

	windowIOController.addSubscriber(&imageBackground);
	windowIOController.addSubscriber(&ui);

	while (true)
	{
		windowIOController.pollEvents();
		audioPlayer.pollStatus();

		// Clear previous frame.
		SDL_RenderClear(renderer);

		// Draw background.
		imageBackground.draw();

		// Draw UI.
		ui.draw();

		// Render current frame.
		SDL_RenderPresent(renderer);
	}

	// Free fonts.
	PFonts::freeFonts();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return -1;
}
