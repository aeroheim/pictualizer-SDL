#include "ImageTexture.h"

ImageTexture::ImageTexture()
{
	image = NULL;
	iw = 0;
	ih = 0;
}

ImageTexture::~ImageTexture()
{
	if (image)
		SDL_DestroyTexture(image);
}

void ImageTexture::draw(SDL_Renderer* ren, SDL_Rect* view)
{ 
	if (image)
		SDL_RenderCopy(ren, image, view, NULL);
}

int ImageTexture::getWidth()
{
	return iw;
}

int ImageTexture::getHeight()
{
	return ih;
}

bool ImageTexture::setImage(SDL_Renderer* ren, std::string path)
{
	image = IMG_LoadTexture(ren, path.c_str());

	if (image)
	{
		SDL_QueryTexture(image, NULL, NULL, &iw, &ih);
		return true;
	}

	iw = 0;
	ih = 0;
	return false;
}

void ImageTexture::freeImage()
{
	if (image)
	{
		SDL_DestroyTexture(image);
		image = NULL;
		iw = 0;
		ih = 0;
	}
}

void ImageTexture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(image, r, g, b);
}

void ImageTexture::getColor(Uint8* r, Uint8* g, Uint8* b)
{
	SDL_GetTextureColorMod(image, r, g, b);
}

void ImageTexture::setTint(Uint8 rgb)
{
	setColor(rgb, rgb, rgb);
}

void ImageTexture::setBlur(int level)
{

}

void ImageTexture::getAlpha(Uint8* alpha)
{
	SDL_GetTextureAlphaMod(image, alpha);
}

void ImageTexture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(image, alpha);
}

void ImageTexture::setBlendMode(SDL_BlendMode blend)
{
	SDL_SetTextureBlendMode(image, blend);
}