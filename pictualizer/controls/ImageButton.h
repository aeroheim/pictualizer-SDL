#pragma once

#include "Image.h"
#include "Button.h"

class ImageButton : public Button
{
	public:
		ImageButton(SDL_Texture* image, float x, float y, float w, float h);
		~ImageButton();

		void setImage(SDL_Texture* image);

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

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
		Image image;
		bool mouseOver;
		void OnMouseMotion(MouseMotionEvent* e);
};