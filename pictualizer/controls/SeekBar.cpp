#include "SeekBar.h"
#include <iostream>

SeekBar::SeekBar(SDL_Renderer* ren, TTF_Font* font, float x, float y, float w, float h) :
	PControl(x, y, w, h),
	ren(ren),
	bodyGrid(x, y, std::vector <float> { h * (3.0f / 4.0f), h * (1.0f / 4.0f)}, std::vector <float> { w }),
	timeLabel(font, x, y, w, h),
	bar(ren, x, y, w, h),
	durationString("00:00:00"),
	duration(0),
	time(0)
{
	timeLabel.setText("00:00:00 / 00:00:00", ren);

	bodyGrid[0][0].setElement(&timeLabel);
	bodyGrid[1][0].setElement(&bar);
	bodyGrid[1][0].setPadding(0, 1, 0, 3);
}

SeekBar::~SeekBar() {}

void SeekBar::setTime(int seconds)
{
	assert(seconds <= duration);

	time = seconds;

	bar.setProgress((float) time);

	int h = (seconds / 3600);
	int m = (seconds / 60) % 60;
	int s = seconds % 60;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << h << ':' << std::setw(2) << std::setfill('0') << m << ':' << std::setw(2) << std::setfill('0') << s << " / ";
	timeLabel.setText(ss.str().append(durationString), ren);
}

void SeekBar::setDuration(int seconds)
{
	assert(seconds >= time);

	duration = seconds;

	bar.setMaxProgress((float) seconds);

	int h = (seconds / 3600);
	int m = (seconds / 60) % 60;
	int s = seconds % 60;

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << h << ':' << std::setw(2) << std::setfill('0') << m << ':' << std::setw(2) << std::setfill('0') << s;
	durationString = ss.str();

	setTime(this->time);
}

void SeekBar::setX(float x)
{
	PControl::setX(x);
	bodyGrid.setX(x);
}

void SeekBar::setY(float y)
{
	PControl::setY(y);
	bodyGrid.setY(y);
}

void SeekBar::setWidth(float w)
{
	PControl::setWidth(w);
	bodyGrid.setWidth(w);
}

void SeekBar::setHeight(float h)
{
	PControl::setHeight(h);
	bodyGrid.setHeight(h);
}

void SeekBar::setLabelColor(float r, float g, float b)
{
	timeLabel.setColor(r, g, b);
}

PFloatColor SeekBar::getLabelColor()
{
	return timeLabel.getColor();
}

void SeekBar::setBaseColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
	bar.setBaseColor(r, g, b);
}

void SeekBar::setProgressColor(float r, float g, float b)
{
	bar.setProgressColor(r, g, b);
}

PFloatColor SeekBar::getProgressColor()
{
	return bar.getProgressColor();
}

void SeekBar::setColor(float r, float g, float b)
{
	setBaseColor(r, g, b);
}

void SeekBar::setTint(float t)
{
	PControl::setTint(t);
	bodyGrid.setTint(t);
}

void SeekBar::setBaseTint(float t)
{
	PControl::setBaseTint(t);
	bodyGrid.setBaseTint(t);
}

void SeekBar::setFocusTint(float t)
{
	PControl::setFocusTint(t);
	bodyGrid.setFocusTint(t);
}

void SeekBar::setTintState(PControlTintState s)
{
	PControl::setTintState(s);
	bodyGrid.setTintState(s);
}

void SeekBar::setTintDelta(float delta)
{
	PControl::setTintDelta(delta);
	bodyGrid.setTintDelta(delta);
}

void SeekBar::setAlpha(float a)
{
	PControl::setAlpha(a);
	bodyGrid.setAlpha(a);
}

void SeekBar::setMinAlpha(float a)
{
	PControl::setMinAlpha(a);
	bodyGrid.setMinAlpha(a);
}

void SeekBar::setMaxAlpha(float a)
{
	PControl::setMaxAlpha(a);
	bodyGrid.setMaxAlpha(a);
}

void SeekBar::setFadeState(PControlFadeState s)
{
	PControl::setFadeState(s);
	bodyGrid.setFadeState(s);
}

void SeekBar::setFadeDelta(float delta)
{
	PControl::setFadeDelta(delta);
	bodyGrid.setFadeDelta(delta);
}

void SeekBar::draw(SDL_Renderer* ren = nullptr)
{
	if (ren)
		bodyGrid.draw(ren);
	else
		bodyGrid.draw(this->ren);

	PControl::draw(nullptr);
}

void SeekBar::handleEvent(Event* e)
{
	if (!e->handled)
	{
		if (MouseDownEvent* mouseDownEvent = dynamic_cast<MouseDownEvent*>(e))
		{
			if (bodyGrid.mouseInside(mouseDownEvent->x, mouseDownEvent->y) && !lMouseHeld && mouseDownEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = true;
				mDownX = mouseDownEvent->x;
				mDownY = mouseDownEvent->y;

				OnMouseDown(mouseDownEvent);

				e->handled = true;
			}
		}
		else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
		{
			if (lMouseHeld && mouseUpEvent->button == SDL_BUTTON_LEFT)
			{
				lMouseHeld = false;
				OnMouseUp(mouseUpEvent);

				e->handled = true;
			}
		}
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		{
			if (lMouseHeld)
			{
				OnMouseMotion(mouseMotionEvent);
				e->handled = true;
			}
		}
	}
}

void SeekBar::OnMouseDown(MouseDownEvent* e)
{
	int seekTime = (int) std::round(((e->x - bar.getX()) / bar.getWidth()) * duration);
	seekTime = seekTime < 0 ? 0 : seekTime;

	setTime(seekTime);
}

void SeekBar::OnMouseUp(MouseUpEvent* e)
{
	int mouseX = e->x > bar.getX() + bar.getWidth() ? (int) (bar.getX() + bar.getWidth()) : (e->x < bar.getX() ? (int) bar.getX() : e->x);

	int seekTime = (int) std::round(((mouseX - bar.getX()) / bar.getWidth()) * duration);
	seekTime = seekTime < 0 ? 0 : seekTime;

	setTime(seekTime);

	TrackSeekEvent trackSeekEvent(seekTime);
	notify(&trackSeekEvent);
}

void SeekBar::OnMouseMotion(MouseMotionEvent* e)
{
	int mouseX = e->x > bar.getX() + bar.getWidth() ? (int) (bar.getX() + bar.getWidth()) : (e->x < bar.getX() ? (int) bar.getX() : e->x);

	int seekTime = (int) std::round(((mouseX - bar.getX()) / bar.getWidth()) * duration);
	seekTime = seekTime < 0 ? 0 : seekTime;

	setTime(seekTime);
}