#include "Label.h"

Label::Label(TTF_Font* font, float x, float y, float w, float h) : font(font), PControl(x, y, w, h)
{
	clipState = LabelClipState::CLIP;
	alignState = LabelAlignState::LEFT;

	textIsPannable = false;
	panSpeed = SRC_PAN_SPEED;
	resetPanning();

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = SDL_ALPHA_OPAQUE;

	dest.x = getRoundedX();
	dest.y = getRoundedY();
	dest.w = getRoundedWidth();
	dest.h = getRoundedHeight();

	view.x = 0;
	view.y = 0;
	view.w = 0;
	view.h = 0;

	texture = nullptr;
}

void Label::setX(float x)
{
	PControl::setX(x);

	float midpoint = x + (getWidth() / 2.0f);
	float textRadius = dest.w / 2.0f;

	switch (alignState)
	{
		case LabelAlignState::LEFT:
			dest.x = getRoundedX();
			break;
		case LabelAlignState::CENTER:
			dest.x = (int) std::round(midpoint - textRadius);
			break;
		case LabelAlignState::RIGHT:
			dest.x = (int) std::round(x + (getWidth() - dest.w));
			break;
	}
}

void Label::setY(float y)
{
	PControl::setY(y);
	dest.y = getRoundedY();
}
 
void Label::setWidth(float w)
{
	PControl::setWidth(w);

	if (!text.empty())
		resetView();
	else
		dest.w = getRoundedWidth();
}

void Label::setHeight(float h)
{
	PControl::setHeight(h);

	if (!text.empty())
		resetView();

	dest.h = getRoundedHeight();
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

void Label::setClipState(LabelClipState s)
{
	clipState = s;

	switch (s)
	{
		case LabelClipState::PAN:
			resetPanning();
			break;
		case LabelClipState::STRETCH:
			resetView();
			break;
		case LabelClipState::CLIP:
			resetView();
			break;
		default:
			break;
	}
}

LabelClipState Label::getClipState()
{
	return clipState;
}

void Label::setAlignState(LabelAlignState s)
{
	alignState = s;

	if (clipState == LabelClipState::CLIP)
		resetView();
}

LabelAlignState Label::getAlignState()
{
	return alignState;
}

void Label::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (texture)
		SDL_SetTextureColorMod(texture, (int) std::round(r), (int) std::round(g), (int) std::round(b));
}

void Label::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (texture)
		SDL_SetTextureAlphaMod(texture, getRoundedAlpha());
}


void Label::draw(SDL_Renderer* ren)
{
	if (texture)
	{
		SDL_RenderCopy(ren, texture, &view, &dest);

		if (clipState == LabelClipState::PAN && textIsPannable)
		{
			if (panStopped)
				OnPanStopped();
			else
				panView();
		}
	}

	PControl::draw(nullptr);
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

	float textScale = view.h / getHeight();
	float whratio = getWidth() / getHeight();
	float scaledTextWidth = view.w / textScale;

	// Clip texture view if wider than label width.
	if (scaledTextWidth > getWidth())
	{
		textIsPannable = true;
		maxPanX = (int) std::floor((scaledTextWidth - getWidth()) * textScale);

		if (clipState == LabelClipState::PAN || clipState == LabelClipState::CLIP)
			view.w = (int) std::round(view.h * whratio);

		dest.w = getRoundedWidth();
	}
	// Otherwise prevent text shorter than the label width from stretching.
	else
	{
		textIsPannable = false;

		if (clipState == LabelClipState::STRETCH)
			dest.w = getRoundedWidth();
		else
		{
			dest.w = (int) std::round((float) view.w / textScale);

			float midpoint = getX() + (getWidth() / 2.0f);
			float textRadius = dest.w / 2.0f;

			// Align text to the correct position.
			switch (alignState)
			{
				case LabelAlignState::LEFT:
					dest.x = getRoundedX();
					break;
				case LabelAlignState::CENTER:
					dest.x = (int) std::round(midpoint - textRadius);
					break;
				case LabelAlignState::RIGHT:
					dest.x = (int) std::round(getX() + (getWidth() - dest.w));
					break;
			}
		}
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