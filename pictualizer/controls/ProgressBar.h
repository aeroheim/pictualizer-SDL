#pragma once

#include "Image.h"
#include "PControl.h"

class ProgressBar : public PControl
{
	public:
		ProgressBar(SDL_Renderer* ren, float x, float y, float w, float h);

		void setProgress(float units);
		void setMaxProgress(float units);

		float getUnitProgress();
		float getMaxUnitProgress();
		float getPercentProgress();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);
		
		void setBaseColor(float r, float g, float b);
		void setProgressColor(float r, float g, float b);
		PFloatColor getProgressColor();
		void setColor(float r, float g, float b);

		void setTint(float t);
		void setBaseTint(float t);
		void setFocusTint(float t);
		void setTintState(PControlTintState s);
		void setTintDelta(float delta);

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeDelta(float delta);
		
		void draw(SDL_Renderer* ren);

	private:
		Image baseBar;
		Image progressBar;
		float progress;
		float maxProgress;
};