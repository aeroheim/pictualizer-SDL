#pragma once

#include "PTexture.h"
#include "Button.h"

class PTextureButton : public Button
{
	public:
		PTextureButton(PTextureType, float x, float y, float w, float h);
		~PTextureButton();

		void setTexture(PTextureType textureType);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void setColor(float r, float g, float b);
		void setBaseColor(float r, float g, float b);
		void setFocusColor(float r, float g, float b);
		void setColorState(PControlColorState s);
		void setColorStyle(PControlColorStyle s);
		void setColorSpeed(float seconds);

		void setAlpha(float a);
		void setMinAlpha(float a);
		void setMaxAlpha(float a);
		void setFadeState(PControlFadeState s);
		void setFadeStyle(PControlFadeStyle s);
		void setFadeSpeed(float speed);

		void draw(SDL_Renderer* ren);

		void handleEvent(Event* e);

	private:
		PTexture texture;
		bool mouseOver;
		void OnMouseDown(MouseDownEvent* e);
		void OnMouseUp(MouseUpEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
};