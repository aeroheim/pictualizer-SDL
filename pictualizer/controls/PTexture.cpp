#include "PTexture.h"

PTexture::PTexture(PTextureType textureType, float x, float y, float w, float h) :
	PControl(x, y, w, h),
	textureType(textureType),
	texture(nullptr)
{
	PTextures::requestTexture(textureType, (int) std::round(getWidth() * getHeight()), &texture);

	dest.x = getRoundedX();
	dest.y = getRoundedY();
	dest.w = getRoundedWidth();
	dest.h = getRoundedHeight();
}

PTexture::PTexture(float x, float y, float w, float h) : PTexture(PTextureType::NONE, x, y, w, h) {}

PTexture::~PTexture()
{
	if (texture)
		SDL_DestroyTexture(texture);
}

PTexture::PTexture(const PTexture& other) :
	PControl(other),
	textureType(other.textureType),
	dest(other.dest)
{
	if (PTextures::requestTexture(textureType, (int) std::round(getWidth() * getHeight()), &texture))
	{
		// Restore previous color & alpha values
		PFloatColor color = getColor();

		setColor(color.r, color.g, color.b);
		setAlpha(getAlpha());
	}
}

PTexture& PTexture::operator=(const PTexture& other)
{
	setX(((PControl&) other).getX());
	setY(((PControl&) other).getY());
	setWidth(((PControl&) other).getWidth());
	setHeight(((PControl&) other).getHeight());

	if (PTextures::requestTexture(other.textureType, (int) std::round(getWidth() * getHeight()), &texture, textureType))
	{
		// Restore previous color & alpha values
		PFloatColor color = getColor();

		setColor(color.r, color.g, color.b);
		setAlpha(getAlpha());
	}

	textureType = other.textureType;
	dest = other.dest;

	return *this;
}

void PTexture::setTexture(PTextureType textureType)
{
	if (PTextures::requestTexture(textureType, (int) std::round(getWidth() * getHeight()), &texture, this->textureType))
	{
		// Restore previous color & alpha values
		PFloatColor color = getColor();

		setColor(color.r, color.g, color.b);
		setAlpha(getAlpha());
	}

	this->textureType = textureType;
}

void PTexture::freeTexture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		textureType = PTextureType::NONE;
	}
}

void PTexture::setX(float x)
{
	PControl::setX(x);
	dest.x = getRoundedX();
}

void PTexture::setY(float y)
{
	PControl::setY(y);
	dest.y = getRoundedY();
}

void PTexture::setWidth(float w)
{
	PControl::setWidth(w);
	dest.w = getRoundedWidth();

	if (PTextures::requestTexture(textureType, (int) std::round(getWidth() * getHeight()), &texture, this->textureType))
	{
		// Restore previous color & alpha values
		PFloatColor color = getColor();

		setColor(color.r, color.g, color.b);
		setAlpha(getAlpha());
	}
}

void PTexture::setHeight(float h)
{
	PControl::setHeight(h);
	dest.h = getRoundedHeight();

	if (PTextures::requestTexture(textureType, (int) std::round(getWidth() * getHeight()), &texture, this->textureType))
	{
		// Restore previous color & alpha values
		PFloatColor color = getColor();

		setColor(color.r, color.g, color.b);
		setAlpha(getAlpha());
	}
}

void PTexture::setColor(float r, float g, float b)
{
	PControl::setColor(r, g, b);

	if (texture)
	{
		PIntColor color = getRoundedColor();
		SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
	}
}

void PTexture::setAlpha(float a)
{
	PControl::setAlpha(a);

	if (texture)
		SDL_SetTextureAlphaMod(texture, getRoundedAlpha());
}

void PTexture::draw(SDL_Renderer* ren)
{
	if (texture)
		SDL_RenderCopy(ren, texture, nullptr, &dest);

	PControl::draw(ren);
}
