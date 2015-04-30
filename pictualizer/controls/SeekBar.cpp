#include "SeekBar.h"

SeekBar::SeekBar(SDL_Renderer* ren, PFontType fontType, float x, float y, float w, float h) :
	PControl(x, y, w, h),
	bodyGrid(x, y, std::vector <float> { h * (3.0f / 4.0f), h * (1.0f / 4.0f)}, std::vector <float> { w }),
	timeLabel(ren, fontType, x, y, w, h),
	bar(ren, x, y, w, h),
	durationString("00:00:00"),
	duration(0),
	time(0)
{
	timeLabel.setText("00:00:00 / 00:00:00");

	bodyGrid[0][0].setElement(&timeLabel);
	bodyGrid[0][0].setPadding(0, 0.1f, 0, 0);
	bodyGrid[1][0].setElement(&bar);
	bodyGrid[1][0].setPadding(0, 0, 0, 0.85f);
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
	timeLabel.setText(ss.str().append(durationString));
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

void SeekBar::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);
	setLabelColor(r, g, b);
	setBarColor(r, g, b);
}

void SeekBar::setLabelColor(float r, float g, float b)
{
	timeLabel.setColor(r, g, b);
}

void SeekBar::setBarColor(float r, float g, float b)
{
	bar.setBarColor(r, g, b);
}

void SeekBar::setProgressColor(float r, float g, float b)
{
	bar.setProgressColor(r, g, b);
}

const PFloatColor& SeekBar::getLabelColor()
{
	return timeLabel.getColor();
}

PIntColor SeekBar::getRoundedLabelColor()
{
	return timeLabel.getRoundedColor();
}

const PFloatColor& SeekBar::getBarColor()
{
	return bar.getBarColor();
}

PIntColor SeekBar::getRoundedBarColor()
{
	return bar.getRoundedBarColor();
}

const PFloatColor& SeekBar::getProgressColor()
{
	return bar.getProgressColor();
}

PIntColor SeekBar::getRoundedProgressColor()
{
	return bar.getRoundedProgressColor();
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

void SeekBar::setFadeStyle(PControlFadeStyle s)
{
	PControl::setFadeStyle(s);
	bodyGrid.setFadeStyle(s);
}

void SeekBar::setFadeSpeed(float seconds)
{
	PControl::setFadeSpeed(seconds);
	bodyGrid.setFadeSpeed(seconds);
}

void SeekBar::draw(SDL_Renderer* ren)
{
	bodyGrid.draw(ren);

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

	SeekEvent seekEvent;
	notify(&seekEvent);
}

void SeekBar::OnMouseUp(MouseUpEvent* e)
{
	int mouseX = e->x > bar.getX() + bar.getWidth() ? (int) (bar.getX() + bar.getWidth()) : (e->x < bar.getX() ? (int) bar.getX() : e->x);

	int seekTime = (int) std::round(((mouseX - bar.getX()) / bar.getWidth()) * duration);
	seekTime = seekTime < 0 ? 0 : seekTime;

	setTime(seekTime);

	SeekRequestEvent seekRequestEvent(seekTime);
	notify(&seekRequestEvent);
}

void SeekBar::OnMouseMotion(MouseMotionEvent* e)
{
	int mouseX = e->x > bar.getX() + bar.getWidth() ? (int) (bar.getX() + bar.getWidth()) : (e->x < bar.getX() ? (int) bar.getX() : e->x);

	int seekTime = (int) std::round(((mouseX - bar.getX()) / bar.getWidth()) * duration);
	seekTime = seekTime < 0 ? 0 : seekTime;

	setTime(seekTime);
}