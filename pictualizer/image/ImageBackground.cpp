#include "ImageBackground.h"
#include <iostream>

using namespace std;

ImageBackground::ImageBackground(SDL_Renderer* ren, int ww, int wh) : ren(ren), imageCamera(ww, wh), tempCamera(ww, wh)
{
	imgIndex = -1;
	slideshow = false;
	fading = false;
	tempAlpha = SDL_ALPHA_OPAQUE;
	fadeDelta = 15;

	addSubscriber(&imageCamera);
	registerKey(ACCESS_KEY);

	imageCamera.setPanSpeed(0.33f);
	imageCamera.setState(ImageCameraState::ROAMING);
}

ImageBackground::~ImageBackground() {}

void ImageBackground::draw()
{
	if (!images.empty())
	{
		image.draw(ren, imageCamera.getView());

		if (imageCamera.getState() == ImageCameraState::ROAMING)
		{
			imageCamera.updateView();

			if (viewInFadeZone(imageCamera, image))
			{
				cout << "ImageBackground in fade zone." << endl;
				fading = true;
				tempAlpha = SDL_ALPHA_OPAQUE;
				tempCamera = imageCamera;

				imageCamera.resetPanning();

				calculateFadeZone(imageCamera, image);
			}

			if (fading)
			{
				image.setAlpha(tempAlpha);

				fadeImage(image, false);

				image.draw(ren, tempCamera.getView());

				image.getAlpha(&tempAlpha);
				image.setAlpha(SDL_ALPHA_OPAQUE);

				tempCamera.updateView();
			}
		}
		else if (fading)
		{
			fadeImage(temp, false, true);
			temp.draw(ren, tempCamera.getView());
		}
	}

}

void ImageBackground::setImage(std::string path)
{
	// Fade to the new ImageTexture, if there exists an image to fade to.
	if (images.size() > 1)
	{
		fading = true;
		tempCamera = imageCamera;
		temp = image;
	}

	image.setImage(ren, path);
	image.setBlendMode(SDL_BLENDMODE_BLEND);
	imageCamera.setView(&image);
	calculateFadeZone(imageCamera, image);

}

void ImageBackground::setImage(int index)
{
	if (index >= 0 && (size_t)index < images.size())
	{
		setImage(images[index]);
		imgIndex = index;
	}
}

void ImageBackground::nextImage()
{
	if (imgIndex + 1 >= 0 && (size_t) (imgIndex + 1) < images.size())
	{
		imgIndex += 1;
		setImage(images[imgIndex]);
	}
}

void ImageBackground::prevImage()
{
	if (imgIndex - 1 >= 0 && (size_t)(imgIndex - 1) < images.size())
	{
		imgIndex -= 1;
		setImage(images[imgIndex]);
	}
}

void ImageBackground::enqueueImage(std::string path)
{
	images.push_back(path);
}

void ImageBackground::removeImage(int index)
{
	if (index >= 0 && (size_t)index < images.size())
		images.erase(images.begin() + index);
}

void ImageBackground::handleEvent(Event* e)
{
	notify(e);

	if (!e->handled)
	{
		if (dynamic_cast<FileDropEvent*>(e))
		{
			FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e);
			if (utils::pathIsImage(fileDropEvent->path) && !fading)
			{
				enqueueImage(fileDropEvent->path);
				setImage(images.size() - 1);
				e->handled = true;
			}
		}
		else  if (dynamic_cast<KeyDownEvent*>(e))
		{
			KeyDownEvent* keyDownEvent = dynamic_cast<KeyDownEvent*>(e);
			setKeyHeld(keyDownEvent->key);
		}
		else if (dynamic_cast<KeyUpEvent*>(e))
		{
			KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e);
			setKeyReleased(keyUpEvent->key);

			if (keyHeld(ACCESS_KEY))
			{
				if (keyUpEvent->key == PREV_IMG_KEY && !fading)
					prevImage();
				else if (keyUpEvent->key == NEXT_IMG_KEY && !fading)
					nextImage();
			}
		}
	}
}

void ImageBackground::calculateFadeZone(ImageCamera& camera, ImageTexture& img)
{
	int framesToFade = (int) std::round(SDL_ALPHA_OPAQUE / (float) fadeDelta);
	int fadeDist = (int) std::round(framesToFade * camera.getPanSpeed());

	switch (camera.getPanningState())
	{
		case CameraPanningState::LEFT:
			fadeZone = fadeDist;
			break;
		case CameraPanningState::RIGHT:
			fadeZone = img.getWidth() - fadeDist;
			break;
		case CameraPanningState::TOP:
			fadeZone = fadeDist;
			break;
		case CameraPanningState::BOTTOM:
			fadeZone = img.getHeight() - fadeDist;
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			break;
		case CameraPanningState::BOTTOM_LEFT:
			break;
		case CameraPanningState::TOP_RIGHT:
			break;
		case CameraPanningState::TOP_LEFT:
			break;
	}
}

bool ImageBackground::viewInFadeZone(ImageCamera& camera, ImageTexture& img)
{
	SDL_Rect view = *camera.getView();

	switch (camera.getPanningState())
	{
		case CameraPanningState::LEFT:
			if (view.x <= fadeZone)
				return true;
			break;
		case CameraPanningState::RIGHT:
			if (view.x + view.w >= fadeZone)
				return true;
			break;
		case CameraPanningState::TOP:
			if (view.y <= fadeZone)
				return true;
			break;
		case CameraPanningState::BOTTOM:
			if (view.y + view.h >= fadeZone)
				return true;
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			break;
		case CameraPanningState::BOTTOM_LEFT:
			break;
		case CameraPanningState::TOP_RIGHT:
			break;
		case CameraPanningState::TOP_LEFT:
			break;
	}

	return false;
}


void ImageBackground::fadeImage(ImageTexture& img, bool in, bool free)
{
	Uint8 alpha;
	img.getAlpha(&alpha);

	if (in)
		alpha = alpha + fadeDelta <= SDL_ALPHA_OPAQUE ? alpha + fadeDelta : SDL_ALPHA_OPAQUE;
	else
		alpha = alpha - fadeDelta > 0 ? alpha - fadeDelta : 0;

	img.setAlpha(alpha);

	if (!in && alpha == 0)
	{
		fading = false;

		if (free)
			img.freeImage();
	}
	else if (in && alpha == SDL_ALPHA_OPAQUE)
	{
		fading = false;

		if (free)
			img.freeImage();
	}
}