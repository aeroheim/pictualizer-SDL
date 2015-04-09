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
		PControl& operator=(const PControl& other);

		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		bool mouseInside(int x, int y) const;

		void moveTo(float x, float y);
		void resize(float w, float h);

		virtual void setColor(float r, float g, float b);
		PFloatColor getColor() const;
		PIntColor getRoundedColor() const;

		/*
		 *  PControl objects provide support for modifying its own TINT value in the
		 *  range set between BASE TINT and FOCUS TINT. Derived classes should override
		 *  the provided mutators if they contain other PControls, and should always call
		 *  the original PControl provided implementation in theirs.
		 */
		virtual void setTint(float t);
		float getTint() const;
		Uint8 getRoundedTint() const;

		virtual void setBaseTint(float t);
		float getBaseTint() const;

		virtual void setFocusTint(float t);
		float getFocusTint() const;

		virtual void setTintState(PControlTintState s);
		// virtual void setTintStyle(PControlTintStyle s);
		PControlTintState getTintState() const;

		virtual void setTintDelta(float delta);
		float getTintDelta() const;

		/*
		 *  PControl objects provide support for modifying its own ALPHA value in the
		 *  range set between MIN ALPHA and MAX ALPHA. Derived classes should override
		 *  the provided mutators if they contain other PControls, and should always call
		 *  the original PControl provided implementation in theirs.
		 */
		virtual void setAlpha(float a);
		float getAlpha() const;
		Uint8 getRoundedAlpha() const;

		virtual void setMinAlpha(float a);
		float getMinAlpha() const;

		virtual void setMaxAlpha(float a);
		float getMaxAlpha() const;

		virtual void setFadeState(PControlFadeState s);
		// virtual void setFadeStyle(PControlFadeStyle s);
		PControlFadeState getFadeState() const;

		virtual void setFadeDelta(float delta);
		float getFadeDelta() const;

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