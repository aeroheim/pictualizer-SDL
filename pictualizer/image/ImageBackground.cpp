#include "ImageBackground.h"
#include <iostream>

using namespace std;

ImageBackground::ImageBackground(SDL_Renderer* ren, int ww, int wh) : ren(ren), imageCamera(ww, wh), tempCamera(ww, wh)
{
	imageIndex = -1;
	setState(ImageBackgroundState::SLIDESHOW);
	slideshowTimer = 30;
	frameCount = 0;
	fading = false;

	tempAlpha = SDL_ALPHA_OPAQUE;
	fadeDelta = 15;
	fadeStyle = ImageFadeStyle::ALPHA;

	subscribeTo(&image);
	addSubscriber(&imageCamera);
	registerKey(ACCESS_KEY);

	imageCamera.setPanSpeed(0.33f);
	imageCamera.setState(ImageCameraState::ROAMING);

	calculateFadeDist(imageCamera.getPanSpeed());
}

ImageBackground::~ImageBackground() {}

void ImageBackground::draw()
{
	if (!images.empty())
	{
		// Render the current image.
		image.draw(ren, imageCamera.getView());

		if (imageCamera.getState() == ImageCameraState::ROAMING)
		{
			imageCamera.updateView();

			// Begin fading when the roaming imageCamera hits a fade zone; tempImage is used to facilitate the fade effect.
			if (viewInFadeZone(imageCamera, image) && !fading)
			{
				fading = true;
				tempAlpha = SDL_ALPHA_OPAQUE;
				tempCamera = imageCamera;
				tempImage = image;
				imageCamera.resetPanning();
			}

			// For roaming mode fading, we need to both modify tempImage's alpha and tempCamera's position.
			if (fading)
			{
				// When fading only with the current image, tempImage and image are the same, so we need tempAlpha to store our 
				// current fading alpha value while we draw image with SDL_ALPHA_OPAQUE.
				tempImage.setAlpha(tempAlpha);
				fadeImage(tempImage, false);

				if (tempImage.hasImage())
				{
					tempImage.draw(ren, tempCamera.getView());
					tempImage.getAlpha(&tempAlpha);
					image.setAlpha(SDL_ALPHA_OPAQUE);
					tempCamera.updateView();
				}
			}
		}
		// Manual mode fade case, which is only initiated by setImage().
		else if (fading)
		{
			fadeImage(tempImage, false);

			if (tempImage.hasImage())
				tempImage.draw(ren, tempCamera.getView());
		}

		// Check every frame against the slideshow timer when in slideshow mode.
		if (state == ImageBackgroundState::SLIDESHOW)
			checkSlideshowTimer();
	}
}

void ImageBackground::setState(ImageBackgroundState s)
{
	state = s;

	if (s == ImageBackgroundState::SLIDESHOW)
		frameCount = 0;
}

void ImageBackground::setImage(std::string path)
{
	image.setImage(ren, path);
}

void ImageBackground::setImage(int index)
{
	if (index >= 0 && (size_t)index < images.size())
	{
		setImage(images[index]);
		imageIndex = index;
	}
}

void ImageBackground::nextImage()
{
	if (imageIndex + 1 >= 0 && (size_t) (imageIndex + 1) < images.size())
	{
		imageIndex += 1;
		setImage(images[imageIndex]);
	}
}

void ImageBackground::prevImage()
{
	if (imageIndex - 1 >= 0 && (size_t) (imageIndex - 1) < images.size())
	{
		imageIndex -= 1;
		setImage(images[imageIndex]);
	}
}

void ImageBackground::enqueueImage(std::string path)
{
	images.push_back(path);
}

void ImageBackground::removeImage(int index)
{
	if (index >= 0 && (size_t) index < images.size())
	{
		if (imageIndex == index)
			setImage(index - 1);
		else if (imageIndex < index)
			--imageIndex;

		images.erase(images.begin() + index);
	}
}

void ImageBackground::handleEvent(Event* e)
{
	notify(e);

	if (!e->handled)
	{
		if (FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e))
		{
			if (PUtils::pathIsImage(fileDropEvent->path))
			{
				enqueueImage(fileDropEvent->path);
				setImage(images.size() - 1);
				e->handled = true;
			}
		}
		else if (ImageLoadReadyEvent* imageLoadReadyEvent = dynamic_cast<ImageLoadReadyEvent*>(e))
			OnImageReady();
		else if (ImageLoadedEvent* imageLoadedEvent = dynamic_cast<ImageLoadedEvent*>(e))
			OnImageLoaded();
		else  if (KeyDownEvent* keyDownEvent = dynamic_cast<KeyDownEvent*>(e))
			setKeyHeld(keyDownEvent->key);
		else if (KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e))
		{
			setKeyReleased(keyUpEvent->key);

			if (keyHeld(ACCESS_KEY))
			{
				if (keyUpEvent->key == PREV_IMG_KEY)
					prevImage();
				else if (keyUpEvent->key == NEXT_IMG_KEY)
					nextImage();
			}
		}
	}
}

void ImageBackground::calculateFadeDist(float panSpeed)
{
	int framesToFade = (int) std::round(SDL_ALPHA_OPAQUE / (float) fadeDelta);
	fadeDist = (int) std::round(framesToFade * panSpeed);
}

bool ImageBackground::viewInFadeZone(ImageCamera& camera, ImageTexture& img)
{
	SDL_Rect view = *camera.getView();

	switch (camera.getPanningState())
	{
		case CameraPanningState::LEFT:
			if (view.x <= fadeDist)
				return true;
			break;
		case CameraPanningState::RIGHT:
			if (view.x + view.w >= img.getWidth() - fadeDist)
				return true;
			break;
		case CameraPanningState::TOP:
			if (view.y <= fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM:
			if (view.y + view.h >= img.getHeight() - fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			if (view.y + view.h >= img.getHeight() - fadeDist || view.x + view.w >= img.getWidth() - fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM_LEFT:
			if (view.y + view.h >= img.getHeight() - fadeDist || view.x <= fadeDist)
				return true;
			break;
		case CameraPanningState::TOP_RIGHT:
			if (view.y <= fadeDist || view.x + view.w >= img.getWidth() - fadeDist)
				return true;
			break;
		case CameraPanningState::TOP_LEFT:
			if (view.y <= fadeDist || view.x <= fadeDist)
				return true;
			break;
	}

	return false;
}

void ImageBackground::checkSlideshowTimer()
{
	if (images.size() > 1)
	{
		++frameCount;

		// Advance to the next image (circular) if the timer has been reached.
		if (((float) frameCount / 60) == slideshowTimer)
		{
			if ((size_t) (imageIndex + 1) < images.size())
				nextImage();
			else
				setImage(0);
		}
	}
}

void ImageBackground::fadeImage(ImageTexture& img, bool in)
{
	Uint8 alpha;
	img.getAlpha(&alpha);

	if (in)
		alpha = alpha + fadeDelta <= SDL_ALPHA_OPAQUE ? alpha + fadeDelta : SDL_ALPHA_OPAQUE;
	else
		alpha = alpha - fadeDelta > 0 ? alpha - fadeDelta : 0;

	img.setAlpha(alpha);

	if (!in && alpha == 0)
		fading = false;
	else if (in && alpha == SDL_ALPHA_OPAQUE)
		fading = false;
}

void ImageBackground::OnImageReady()
{
	// Initiate fading to the new image, if there exists an image to fade to.
	if (images.size() > 1)
	{
		fading = true;
		tempCamera = imageCamera;
		tempImage = image;
	}

	// Reset slideshow timer count.
	frameCount = 0;
}

void ImageBackground::OnImageLoaded()
{
	image.setBlendMode(SDL_BLENDMODE_BLEND);
	imageCamera.setView(&image);

	// Reset alpha, which is necessary if the previous image did not fully fade out before setImage was called again.
	tempAlpha = SDL_ALPHA_OPAQUE;
}