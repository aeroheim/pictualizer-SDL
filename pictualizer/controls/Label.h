#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "PControl.h"

class Label : public PControl
{
	public:
		Label(TTF_Font* font, int x, int y, int w, int h);

		void setX(int x);
		void setY(int y);
		void setWidth(int w);
		void setHeight(int h);
		
		void setFont(TTF_Font* font, SDL_Renderer* ren);
		TTF_Font* getFont();

		void setText(std::string text, SDL_Renderer* ren);
		std::string getText();

		void setColor(Uint8 r, Uint8 g, Uint8 b);
		SDL_Color* getColor();

		void setAlpha(int a);
		Uint8 getAlpha();

		void draw(SDL_Renderer* ren);

	private:
		TTF_Font* font;
		SDL_Texture* texture;
		SDL_Color color;
		SDL_Rect view;
		SDL_Rect dest;
		std::string text;

		void adjustView();
		void loadTexture(SDL_Renderer* ren);
};