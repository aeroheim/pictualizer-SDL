#pragma once

#include <SDL.h>
#include <algorithm>

class PControl
{
	public:
		virtual void setX(float x);
		virtual void setY(float y);
		virtual void setWidth(float w);
		virtual void setHeight(float h);

		float getX();
		float getY();
		float getWidth();
		float getHeight();

		int getRoundedX();
		int getRoundedY();
		int getRoundedWidth();
		int getRoundedHeight();

		void moveTo(float x, float y);
		void resize(float w, float h);

		virtual void draw(SDL_Renderer* ren) = 0;

	protected:
		PControl(float x, float y, float w, float h);
		virtual ~PControl();

	private:
		float x;
		float y;
		float w;
		float h;
};