#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "PControl.h"
#include "LabelStates.h"

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

		void setClipState(LabelClipState s);
		LabelClipState getClipState();

		void setAlignState(LabelAlignState s);
		LabelAlignState getAlignState();

		void setColor(Uint8 r, Uint8 g, Uint8 b);
		SDL_Color* getColor();

		void setAlpha(Uint8 a);
		Uint8 getAlpha();

		void draw(SDL_Renderer* ren);

	private:
		TTF_Font* font;
		SDL_Texture* texture;
		SDL_Color color;
		SDL_Rect view;
		SDL_Rect dest;

		std::string text;

		LabelClipState clipState;
		LabelAlignState alignState;

		const int PAN_WAIT_DURATION = 5;
		const float SRC_PAN_SPEED = 0.0005f;
		float panSpeed;
		float panX;
		int maxPanX;
		int frameCount;
		bool textIsPannable;
		bool panStopped;

		void panView();
		void resetView();
		void getTextTexture(SDL_Renderer* ren);

		void resetPanning();
		void OnPanStopped();
};