#include "ImageButton.h"

ImageButton::ImageButton(ImageTexture image, float x, float y, float w, float h) :
	Image(image, x, y, w, h),
	Button(x, y, w, h)
{}

ImageButton::ImageButton(float x, float y, float w, float h) :
	Image(x, y, w, h),
	Button(x, y, w, h)
{}

ImageButton::~ImageButton() {}
