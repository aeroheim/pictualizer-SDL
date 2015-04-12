#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <locale>
#include <codecvt>
#include "PControl.h"
#include "LabelStates.h"
#include "../assets/fonts/PFonts.h"

class Label : public PControl
{
	public:
		Label(SDL_Renderer* ren, PFontType fontType, float x, float y, float w, float h);
		~Label();

		Label(const Label& other);
		Label& operator=(const Label& other);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);
		
		void setFont(PFontType fontType);
		TTF_Font* getFont();

		void setText(std::string text);
		void setText(std::wstring text);
		std::wstring getText();

		void setClipState(LabelClipState s);
		LabelClipState getClipState();

		void setAlignState(LabelAlignState s);
		LabelAlignState getAlignState();

		void setColor(float r, float g, float b);
		void setAlpha(float a);

		void draw(SDL_Renderer* ren = nullptr);

	private:
		SDL_Renderer* ren;
		PFontType fontType;
		TTF_Font* font;
		SDL_Texture* texture;
		SDL_Color color;
		SDL_Rect view;
		SDL_Rect dest;

		std::wstring text;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> wstrConverter;

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