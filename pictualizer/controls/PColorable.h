#pragma once
#include <SDL_hints.h>

struct PFloatColor
{
	float r;
	float g;
	float b;
	float a;
};

struct PIntColor
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
};

class PColorable
{
	public:
		virtual void setBackgroundColor(float r, float g, float b, float a = 255);
		void setColor(const PFloatColor& c);
		void setColor(const PIntColor& c);
		const PFloatColor& getColor() const;
		PIntColor getRoundedColor() const;

		virtual void setForegroundColor(float r, float g, float b, float a = 255);
		void setBaseColor(const PFloatColor& c);
		void setBaseColor(const PIntColor& c);
		const PFloatColor& getBaseColor() const;
		PIntColor getRoundedBaseColor() const;

		virtual void setFocusColor(float r, float g, float b, float a = 255);
		void setFocusColor(const PFloatColor& c);
		void setFocusColor(const PIntColor& c);
		const PFloatColor& getFocusColor() const;
		PIntColor getRoundedFocusColor() const;

	private:
		PFloatColor _backgroundColor;
		PFloatColor _foregroundColor;
		PFloatColor _focusColor;

		void setColorDelta();
};