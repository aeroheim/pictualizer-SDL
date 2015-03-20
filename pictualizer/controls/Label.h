#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "PControl.h"
#include "LabelStates.h"

class Label : public PControl
{
	public:
		Label(TTF_Font* font, float x, float y, float w, float h);
		Label(float x, float y, float w, float h);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);
		
		void setFont(TTF_Font* font, SDL_Renderer* ren);
		TTF_Font* getFont();

		void setText(std::string text, SDL_Renderer* ren);
		std::string getText();

		void setClipState(LabelClipState s);
		LabelClipState getClipState();

		void setAlignState(LabelAlignState s);
		LabelAlignState getAlignState();

		void setColor(float r, float g, float b);
		void setAlpha(float a);

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