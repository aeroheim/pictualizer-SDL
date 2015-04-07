#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "../io/WindowIOController.h"
#include "../image/ImageBackground.h"
#include "../assets/textures/PTextures.h"
#include "../assets/fonts/PFonts.h"
#include "../ui/PUI.h"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	// Init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create window
	SDL_Window* win = SDL_CreateWindow("Pictualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 530, SDL_WINDOW_BORDERLESS);
	if (win == nullptr)
	{
		cout << "Window Error: " << SDL_GetError << endl;
		SDL_Quit();
		return 1;
	}

	// Create renderer
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Initialize SDL_TTF
	if (TTF_Init() < 0)
	{
		SDL_DestroyWindow(win);
		cout << "TTF_Init Error: " << TTF_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	// Use linear filtering to preserve image quality when scaled.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");

	int ww, wh;
	SDL_GetWindowSize(win, &ww, &wh);

	// Initialize fonts.
	PFonts::initFonts();

	// Initialize textures.
	PTextures::initTextures(ren);

	// Initialize image background.
	ImageBackground imageBackground(ren, ww, wh);
	
	// Initialize UI.
	PUI ui(win, ren, ww, wh);

	// Initialize IO.
	WindowIOController windowIOController(win);

	windowIOController.addSubscriber(&imageBackground);
	windowIOController.addSubscriber(&ui);

	while (true)
	{
		windowIOController.pollEvents();

		// Clear previous frame.
		SDL_RenderClear(ren);

		// Draw background.
		imageBackground.draw();

		// Draw UI.
		ui.draw();

		// Render current frame.
		SDL_RenderPresent(ren);
	}

	// Free fonts.
	PFonts::freeFonts();

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	TTF_Quit();
	SDL_Quit();
	return -1;
}