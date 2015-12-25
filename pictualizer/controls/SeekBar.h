#pragma once

#include <iomanip>
#include "Image.h"
#include "Label.h"
#include "PControl.h"
#include "GridPanel.h"
#include "ProgressBar.h"
#include "../io/EventObserver.h"
#include "../io/InputListener.h"

class SeekBar : public PControl, public EventObserver, public EventSubscriber, public InputListener
{
	public:
		SeekBar(SDL_Renderer* ren, PFontType fontType, float x, float y, float w, float h);
		~SeekBar();

		void setTime(int seconds);
		void setDuration(int seconds);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);
		void setLabelColor(float r, float g, float b);
		void setBarColor(float r, float g, float b);
		void setProgressColor(float r, float g, float b);
		const PFloatColor& getLabelColor();
		PIntColor getRoundedLabelColor();
		const PFloatColor& getBarColor();
		PIntColor getRoundedBarColor();
		const PFloatColor& getProgressColor();
		PIntColor getRoundedProgressColor();

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeStyle(PControlFadeStyle s);
		void setFadeSpeed(float speed);

		void draw(SDL_Renderer* ren);

		void handleEvent(Event* e);

	private:
		GridPanel bodyGrid;
		Label timeLabel;
		ProgressBar bar;

		std::string durationString;

		int duration;
		int time;

		void OnMouseDown(MouseDownEvent* e);
		void OnMouseUp(MouseUpEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
};