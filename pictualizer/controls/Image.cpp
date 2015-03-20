#include "Image.h"

Image::Image(SDL_Texture* image, float x, float y, float w, float h) : PControl(x, y, w, h)
{
	this->image = image;
	dest.x = getRoundedX();
	dest.y = getRoundedY();
	dest.w = getRoundedWidth();
	dest.h = getRoundedHeight();
}

Image::~Image()
{
	if (image)
		SDL_DestroyTexture(image);
}

void Image::setImage(SDL_Texture* image)
{
	if (this->image)
		SDL_DestroyTexture(this->image);

	this->image = image;
}

void Image::setImage(std::string path, SDL_Renderer* ren)
{
	if (this->image)
		SDL_DestroyTexture(this->image);

	this->image = IMG_LoadTexture(ren, path.c_str());
}

void Image::freeImage()
{
	if (image)
		SDL_DestroyTexture(image);
}

void Image::setX(float x)
{
	PControl::setX(x);
	dest.x = getRoundedX();
}

void Image::setY(float y)
{
	PControl::setY(y);
	dest.y = getRoundedY();
}

void Image::setWidth(float w)
{
	PControl::setWidth(w);
	dest.w = getRoundedWidth();
}

void Image::setHeight(float h)
{
	PControl::setHeight(h);
	dest.h = getRoundedHeight();
}

void Image::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (image)
	{
		PIntColor color = getRoundedColor();
		SDL_SetTextureColorMod(image, color.r, color.g, color.b);
	}
}

void Image::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (image)
		SDL_SetTextureAlphaMod(image, getRoundedAlpha());
}

void Image::draw(SDL_Renderer* ren)
{
	if (image)
		SDL_RenderCopy(ren, image, nullptr, &dest);

	PControl::draw(nullptr);
}
