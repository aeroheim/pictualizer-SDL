#pragma once

#include "Image.h"
#include "Button.h"

class ImageButton : public Image, public Button
{
	public:
		ImageButton(ImageTexture image, float x, float y, float w, float h);
		ImageButton(float x, float y, float w, float h);
		~ImageButton();
};