#include <SDL.h>
#include <iostream>
#include <memory>
#include "../io/WindowIOController.h"
#include "../image/ImageBackground.h"

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

	WindowIOController windowIOController(win);

	int ww, wh;
	SDL_GetWindowSize(win, &ww, &wh);
	ImageBackground imgBg(ren, ww, wh);

	windowIOController.addSubscriber(&imgBg);

	// Use linear filtering to preserve image quality when scaled.
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "LINEAR");

	while (true)
	{
		windowIOController.pollEvents();


		SDL_RenderClear(ren);

		// Draw the texture
		imgBg.draw();

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return -1;
}