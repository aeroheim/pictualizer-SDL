#pragma once

#include <SDL.h>
#include <SDL_main.h>
#include <SDL_syswm.h>
#include <SDL_ttf.h>
#include <iostream>
#include "../controls/PWindow.h"
#include "../image/ImageBackground.h"
#include "../assets/textures/PTextures.h"
#include "../assets/fonts/PFonts.h"
#include "../ui/PUI.h"
#include "../io/EventManager.h"
#include "../assets/cursors/PCursors.h"

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

	// 1080 x 530

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

	// Initialize components.
	PFonts::initFonts();
	PCursors::initCursors();
	PTextures::initTextures(renderer);

	// Initialize image background.
	ImageBackground imageBackground(renderer, ww, wh);
	
	// Initialize audio player.
	AudioPlayer audioPlayer;
	audioPlayer.setVolume(0.5f);

	// Initialize UI.
	PUI ui(renderer, &audioPlayer, ww, wh);

	// Initialize IO.
	PWindow windowIOController(window);

	windowIOController.addSubscriber(&imageBackground);
	windowIOController.addSubscriber(&ui);

	EventManager& eventManager = EventManager::getInstance();
	// eventManager.addSubscriber(&mainWindow);
	// mainWindow.addChild(&ui);

	while (true)
	{
		// windowIOController.pollEvents();
		eventManager.pollEvents();
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

	PFonts::freeFonts();
	PCursors::freeCursors();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	return -1;
}
