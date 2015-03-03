#pragma once

#include <SDL.h>

class PControl
{
	public:
		virtual void setX(int x);
		virtual void setY(int y);
		virtual void setWidth(int w);
		virtual void setHeight(int h);

		int getX();
		int getY();
		int getWidth();
		int getHeight();

		void moveTo(int x, int y);
		virtual void resize(int w, int h);

		virtual void draw(SDL_Renderer* ren) = 0;

	protected:
		PControl(int x, int y, int w, int h);
		virtual ~PControl();

		int x;
		int y;
		int w;
		int h;
};