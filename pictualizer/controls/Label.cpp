#include "Label.h"
#include <iostream>

using namespace std;

Label::Label(TTF_Font* font, int x, int y, int w, int h) : font(font), PControl(x, y, w, h)
{
	state = LabelClippingState::CLIP;
	textIsPannable = false;
	panSpeed = SRC_PAN_SPEED;
	resetPanning();

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
		resetView();
	else
		dest.w = w;
}

void Label::setHeight(int h)
{
	this->h = h;

	if (!text.empty())
		resetView();
	else
		dest.h = h;
}

void Label::setFont(TTF_Font* font, SDL_Renderer* ren)
{
	this->font = font;
	getTextTexture(ren);
}

TTF_Font* Label::getFont()
{
	return font;
}

void Label::setText(std::string text, SDL_Renderer* ren)
{
	this->text = text;
	getTextTexture(ren);
}

std::string Label::getText()
{
	return text;
}

void Label::setClipState(LabelClippingState s)
{
	state = s;

	switch (s)
	{
		case LabelClippingState::PAN:
			resetPanning();
			break;
		case LabelClippingState::STRETCH:
			resetView();
			break;
		case LabelClippingState::CLIP:
			resetView();
			break;
		default:
			break;
	}
}

LabelClippingState Label::getClipState()
{
	return state;
}

void Label::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	color.r = r;
	color.g = g;
	color.b = b;

	if (texture)
		SDL_SetTextureColorMod(texture, r, g, b);
}

SDL_Color* Label::getColor()
{
	return &color;
}

void Label::setAlpha(Uint8 a)
{
	color.a = a;

	if (texture)
		SDL_SetTextureAlphaMod(texture, a);
}

Uint8 Label::getAlpha()
{
	return color.a;
}

void Label::draw(SDL_Renderer* ren)
{
	if (texture)
	{
		SDL_RenderCopy(ren, texture, &view, &dest);

		if (state == LabelClippingState::PAN && textIsPannable)
		{
			if (panStopped)
				OnPanStopped();
			else
				panView();
		}
	}
}

void Label::panView()
{
	if (panSpeed > 0)
		panX = panX + panSpeed < maxPanX ? panX + panSpeed : maxPanX;
	else
		panX = panX + panSpeed > 0 ? panX + panSpeed : 0;
	
	if (panX == 0 || panX == maxPanX)
		panStopped = true;

	view.x = (int) std::round(panX);
}

void Label::resetPanning()
{
	view.x = 0;
	panX = 0;

	frameCount = 0;
	panStopped = true;
	panSpeed = std::abs(panSpeed);
}

void Label::OnPanStopped()
{
	++frameCount;

	// After PAN_WAIT_DURATION, continue panning again in the opposite direction.
	if (((float) frameCount / 60) == PAN_WAIT_DURATION)
	{
		frameCount = 0;
		panStopped = false;

		if (view.x == maxPanX)
			panSpeed = -std::abs(panSpeed);
		else
			panSpeed = std::abs(panSpeed);
	}
}

void Label::resetView()
{
	TTF_SizeUTF8(font, text.c_str(), &view.w, &view.h);

	panSpeed = view.w * SRC_PAN_SPEED;

	float textScale = (float) view.h / h;
	float whratio = (float) w / h;
	float scaledTextWidth = (float) view.w / textScale;

	// Clip texture view if wider than label width.
	if (scaledTextWidth > w)
	{
		textIsPannable = true;
		maxPanX = (int) std::floor((scaledTextWidth - w) * textScale);

		if (state == LabelClippingState::PAN || state == LabelClippingState::CLIP)
			view.w = (int) std::round(view.h * whratio);

		dest.w = w;
	}
	// Otherwise prevent text shorter than the label width from stretching.
	else
	{
		textIsPannable = false;

		if (state == LabelClippingState::STRETCH)
			dest.w = w;
		else
			dest.w = (int)std::round((float) view.w / textScale);
	}
}

void Label::getTextTexture(SDL_Renderer* ren)
{
	// Use UTF8 for unicode support and blended rendering for optimal text quality.
	SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);

	if (!surface)
		throw "Label::getTextTexture() failed to load font surface.\n";

	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	// Convert the loaded surface into a more efficient texture.
	texture = SDL_CreateTextureFromSurface(ren, surface);

	// Since either the font or text has changed, the view must be reset.
	resetView();

	// Reset panning values.
	resetPanning();

	SDL_FreeSurface(surface);
}