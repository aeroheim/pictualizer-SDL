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
		
		void setColor(float r, float g, float b);
		void setBarColor(float r, float g, float b);
		void setProgressColor(float r, float g, float b);
		const PFloatColor& getBarColor() const;
		PIntColor getRoundedBarColor() const;
		const PFloatColor& getProgressColor() const;
		PIntColor getRoundedProgressColor() const;

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeStyle(PControlFadeStyle s);
		void setFadeSpeed(float speed);
		
		void draw(SDL_Renderer* ren);

	private:
		Image baseBar;
		Image progressBar;
		float progress;
		float maxProgress;
};