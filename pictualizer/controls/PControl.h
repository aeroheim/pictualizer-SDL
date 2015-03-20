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
	Uint8 r;
	Uint8 g;
	Uint8 b;
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

		virtual void setColor(float r, float g, float b);
		PFloatColor getColor();
		PIntColor getRoundedColor();

		/*
		 *  PControl objects provide support for modifying its own TINT value in the
		 *  range set between BASE TINT and FOCUS TINT. Derived classes should override
		 *  the provided mutators if they contain other PControls, and should always call
		 *  the original PControl provided implementation in theirs.
		 */
		virtual void setTint(float t);
		float getTint();
		Uint8 getRoundedTint();

		virtual void setBaseTint(float t);
		float getBaseTint();

		virtual void setFocusTint(float t);
		float getFocusTint();

		virtual void setTintState(PControlTintState s);
		PControlTintState getTintState();

		virtual void setTintDelta(float delta);
		float getTintDelta();

		/*
		 *  PControl objects provide support for modifying its own ALPHA value in the
		 *  range set between MIN ALPHA and MAX ALPHA. Derived classes should override
		 *  the provided mutators if they contain other PControls, and should always call
		 *  the original PControl provided implementation in theirs.
		 */
		virtual void setAlpha(float a);
		float getAlpha();
		Uint8 getRoundedAlpha();

		virtual void setMinAlpha(float a);
		float getMinAlpha();

		virtual void setMaxAlpha(float a);
		float getMaxAlpha();

		virtual void setFadeState(PControlFadeState s);
		PControlFadeState getFadeState();

		virtual void setFadeDelta(float delta);
		float getFadeDelta();

		/*
		 *  Derived classes must call PControl's provided implementation
		 *  when implementing draw() to enable tint and alpha effects.
		 */
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

		PControlTintState tintState;
		float baseTint;
		float focusTint;
		float tintDelta;
		float tint;

		PControlFadeState fadeState;
		float minAlpha;
		float maxAlpha;
		float alphaDelta;
		float alpha;
};