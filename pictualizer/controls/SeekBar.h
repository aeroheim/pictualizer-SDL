#pragma once

#include <iomanip>
#include <sstream>
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

		void setLabelColor(float r, float g, float b);
		PFloatColor getLabelColor();
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