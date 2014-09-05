#include <SDL.h>
#include <iostream>
#include <memory>
#include "WindowIOController.h"

using std::cout;
using std::endl;
using std::shared_ptr;

int main(int argc, char** argv)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	// Create window
	SDL_Window* win = SDL_CreateWindow("Hello World", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_BORDERLESS);
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
	WindowIOController windowIOController(win);
	while (true)
	{
		windowIOController.pollEvents();

		// Draw the texture
		SDL_RenderClear(ren);
		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return -1;
}