#pragma once

#include <SDL.h>
#include <assert.h>
#include <algorithm>
#include "PControlStates.h"

struct PFloatColor
{
	float r;
	float g;
	float b;
};

struct PIntColor
{
	int r;
	int g;
	int b;
};

class PControl
{
	public:
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);

		float getX();
		float getY();
		float getWidth();
		float getHeight();

		int getRoundedX();
		int getRoundedY();
		int getRoundedWidth();
		int getRoundedHeight();

		bool mouseInside(int x, int y);

		void moveTo(float x, float y);
		void resize(float w, float h);
		
		void setColor(float r, float g, float b);
		PFloatColor getColor();
		PIntColor getRoundedColor();

		virtual void setAlpha(float a);
		float getAlpha();
		int getRoundedAlpha();

		virtual void setFadeState(PControlFadeState s);
		PControlFadeState getFadeState();

		virtual void setFadeDelta(float delta);
		float getFadeDelta();

		virtual void draw(SDL_Renderer* ren) = 0;

	protected:
		PControl(float x, float y, float w, float h);
		virtual ~PControl();

	private:
		float x;
		float y;
		float w;
		float h;

		float r;
		float g;
		float b;

		PControlFadeState fadeState;
		float aDelta;
		float a;
};