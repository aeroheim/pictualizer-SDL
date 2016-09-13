#pragma once
#include <SDL_hints.h>
#include "PModifiers.h"
#include <memory>

enum class PFadeState
{
	FADEIN,
	FADEOUT,
	NONE
};

class PFadeable
{
	public:
		virtual ~PFadeable();

		virtual void setAlpha(float alpha);
		float getAlpha() const;
		Uint8 getRoundedAlpha() const;

		virtual void setAlphaRange(float minAlpha, float maxAlpha);
		float getMinAlpha() const;
		float getMaxAlpha() const;

		virtual void setFadeState(PFadeState fadeState);
		void setFader(PModifier* fader);
		PFadeState getFadeState() const;

		virtual void setFadeDuration(float seconds);
		float getFadeDuration() const;

		void fade();

	protected:
		PFadeable();
		virtual void OnFadeFinished(PFadeState previousFadeState);

	private:
		std::unique_ptr<PModifier> _fader;
		PFadeState _fadeState;
		float _minAlpha;
		float _maxAlpha;
		float _currentAlpha;
		float _fadeDurationInSeconds;
		void resetFader() const;
};