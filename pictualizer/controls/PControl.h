#pragma once

#include <SDL.h>
#include <assert.h>
#include <algorithm>
#include "PControlStates.h"
#include "../config/PGlobals.h"

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

		void setMinWidth(float minWidth);
		void setMinHeight(float minHeight);
		void setMaxWidth(float maxWidth);
		void setMaxHeight(float maxHeight);

		float getX() const;
		float getY() const;
		float getWidth() const;
		float getHeight() const;

		float getMinWidth() const;
		float getMinHeight() const;
		float getMaxWidth() const;
		float getMaxHeight() const;

		int getRoundedX() const;
		int getRoundedY() const;
		int getRoundedWidth() const;
		int getRoundedHeight() const;

		bool mouseInside(int x, int y) const;
		virtual bool canResizeWidth(float w) const;
		virtual bool canResizeHeight(float h) const;

		void moveTo(float x, float y);
		void resize(float w, float h);

		/*
		 *  PControl objects provide support for modifying its own COLOR value in the between
		 *	BASE COLOR and FOCUS COLOR. Derived classes should override if necessary, and should
		 *	always call original PControl provided implementation in theirs.
		 */
		virtual void setColor(float r, float g, float b);
		void setColor(const PFloatColor& c);
		void setColor(const PIntColor& c);
		const PFloatColor& getColor() const;
		PIntColor getRoundedColor() const;

		virtual void setBaseColor(float r, float g, float b);
		void setBaseColor(const PFloatColor& c);
		void setBaseColor(const PIntColor& c);
		const PFloatColor& getBaseColor() const;
		PIntColor getRoundedBaseColor() const;

		virtual void setFocusColor(float r, float g, float b);
		void setFocusColor(const PFloatColor& c);
		void setFocusColor(const PIntColor& c);
		const PFloatColor& getFocusColor() const;
		PIntColor getRoundedFocusColor() const;

		virtual void setColorState(PControlColorState s);
		virtual void setColorStyle(PControlColorStyle s);
		PControlColorState getColorState() const;
		PControlColorStyle getColorStyle() const;

		virtual void setColorSpeed(float seconds);
		float getColorSpeed() const;

		/*
		 *  PControl objects provide support for modifying its own ALPHA value in the between
		 *	MIN ALPHA and MAX ALPHA. Derived classes should override if necessary, and should
		 *	always call original PControl provided implementation in theirs.
		 */
		virtual void setAlpha(float a);
		float getAlpha() const;
		Uint8 getRoundedAlpha() const;

		virtual void setMinAlpha(float a);
		float getMinAlpha() const;

		virtual void setMaxAlpha(float a);
		float getMaxAlpha() const;

		virtual void setFadeState(PControlFadeState s);
		virtual void setFadeStyle(PControlFadeStyle s);
		PControlFadeState getFadeState() const;
		PControlFadeStyle getFadeStyle() const;

		virtual void setFadeSpeed(float seconds);
		float getFadeSpeed() const;

		/*
		 *  Derived classes must call PControl's provided implementation
		 *  when implementing draw() to enable color and alpha effects.
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
		float minW;
		float minH;
		float maxW;
		float maxH;

		PControlColorState colorState;
		PControlColorStyle colorStyle;
		PFloatColor baseColor;
		PFloatColor focusColor;
		PFloatColor color;
		float colorSpeed;
		float rDelta;
		float gDelta;
		float bDelta;

		PControlFadeState fadeState;
		PControlFadeStyle fadeStyle;
		float minAlpha;
		float maxAlpha;
		float alpha;
		float fadeSpeed;
		float fadeDelta;

		void setFadeDelta();
		void setColorDelta();
};