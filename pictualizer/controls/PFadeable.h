#pragma once
#include <SDL_hints.h>

enum class PControlFadeState
{
	FADEIN,
	FADEOUT,
	NONE
};

enum class PControlFadeStyle
{
	LINEAR,
	EXPONENTIAL,
	SQRT,
	LOG
};

class PFadeable
{
	public:
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

		virtual void setFadeDuration(float seconds);
		float getFadeDuration() const;

		void fade();

	private:
		PControlFadeState fadeState;
		PControlFadeStyle fadeStyle;
		float minAlpha;
		float maxAlpha;
		float alpha;
		float fadeSpeed;
		float fadeDelta;

		void setFadeDelta();
};