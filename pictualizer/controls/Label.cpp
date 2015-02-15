#include "Label.h"
#include <iostream>

using namespace std;

Label::Label(TTF_Font* font, int x, int y, int w, int h) : font(font), PControl(x, y, w, h)
{
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = SDL_ALPHA_OPAQUE;

	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;

	view.x = 0;
	view.y = 0;
	view.w = 0;
	view.h = 0;

	texture = nullptr;
}

void Label::setX(int x)
{
	this->x = x;
	dest.x = x;
}

void Label::setY(int y)
{
	this->y = y;
	dest.y = y;
}

void Label::setWidth(int w)
{
	this->w = w;

	if (!text.empty())
		adjustView();
	else
		dest.w = w;
}

void Label::setHeight(int h)
{
	this->h = h;

	if (!text.empty())
		adjustView();
	else
		dest.h = h;
}

void Label::setFont(TTF_Font* font, SDL_Renderer* ren)
{
	this->font = font;
	loadTexture(ren);
}

TTF_Font* Label::getFont()
{
	return font;
}

void Label::setText(std::string text, SDL_Renderer* ren)
{
	this->text = text;
	loadTexture(ren);
}

std::string Label::getText()
{
	return text;
}

void Label::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

SDL_Color* Label::getColor()
{
	return &color;
}

void Label::setAlpha(int a)
{
	color.a = a;
}

Uint8 Label::getAlpha()
{
	return color.a;
}

void Label::draw(SDL_Renderer* ren)
{
	if (texture)
		SDL_RenderCopy(ren, texture, &view, &dest);
}

void Label::adjustView()
{
	TTF_SizeUTF8(font, text.c_str(), &view.w, &view.h);

	// Upscale amount from the Label to the text.
	float scale = (float) view.h / h;

	// Aspect ratio of the Label.
	float whratio = (float) w / h;

	// Ensure that texture is clipped by this label's boundaries.
	if (((float) view.w / scale) > w)
		view.w = (int) std::round(view.h * whratio);

	// Adjust dest rect to prevent texture stretching.
	dest.w = (int) std::round((float) view.w / scale);
}

void Label::loadTexture(SDL_Renderer* ren)
{
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);

	if (!surface)
		throw "Label::loadTexture() failed to load font surface.\n";

	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	texture = SDL_CreateTextureFromSurface(ren, surface);

	// Since either the font or text has changed, the view must be re-adjusted.
	adjustView();

	SDL_FreeSurface(surface);
}