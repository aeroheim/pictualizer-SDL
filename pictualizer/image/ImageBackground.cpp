#include "ImageBackground.h"

using namespace PGlobals;

ImageBackground::ImageBackground(SDL_Renderer* ren, int ww, int wh) : 
	ren(ren), 
	imageCamera(ww, wh), 
	tempCamera(ww, wh), 
	image(0.0f, 0.0f, (float) ww, (float) wh), 
	tempImage(0.0f, 0.0f, (float) ww, (float) wh),
	iw(0),
	ih(0)
{
	imageIndex = -1;
	setState(ImageBackgroundState::SLIDESHOW);
	slideshowTimer = 30;
	frameCount = 0;

	background = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ww, wh);
	SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);

	image.setMaxAlpha(PConstants::IMGBG_MAX_ALPHA);
	image.setMinAlpha(PConstants::IMGBG_MIN_ALPHA);
	image.setAlpha(image.getMaxAlpha());
	image.setFadeSpeed(PConstants::IMGBG_ROAM_FADE_SPEED);

	tempImage.setMaxAlpha(PConstants::IMGBG_MAX_ALPHA);
	tempImage.setMinAlpha(PConstants::IMGBG_MIN_ALPHA);
	tempImage.setAlpha(PConstants::IMGBG_MAX_ALPHA);
	tempImage.setFadeSpeed(PConstants::IMGBG_ROAM_FADE_SPEED);
	tempAlpha = 0;

	fadeStyle = ImageFadeStyle::ALPHA;

	subscribeTo(&image);
	addSubscriber(&imageCamera);
	registerKey(PVariables::IMGBG_ACCESS_KEY);

	imageCamera.setPanSpeed(PConstants::IMGBG_PAN_SPEED);
	imageCamera.setState(ImageCameraState::ROAMING);

	calculateFadeDist(imageCamera.getPanSpeed());
}

ImageBackground::~ImageBackground()
{
	if (background)
		SDL_DestroyTexture(background);
}

void ImageBackground::draw()
{
	if (!images.empty())
	{
		// Render to the background buffer instead of main Pictualizer buffer.
		SDL_SetRenderTarget(ren, background);

		// Render the current image.
		image.draw(ren, imageCamera.getView());

		if (imageCamera.getState() == ImageCameraState::ROAMING)
		{
			imageCamera.updateView();

			// Begin fading when the roaming imageCamera hits a fade zone; tempImage is used to facilitate the fade effect.
			if (viewInFadeZone(imageCamera, image) && tempImage.getFadeState() != PControlFadeState::FADEOUT)
			{
				tempCamera = imageCamera;
				tempImage = image;

				tempAlpha = tempImage.getMaxAlpha();
				tempImage.setAlpha(tempImage.getMaxAlpha());
				tempImage.setFadeState(PControlFadeState::FADEOUT);

				imageCamera.resetPanning();
			}

			// Since both tempImage and image share the same texture, we must swap alpha values back and forth when fading out.
			if (tempImage.getFadeState() == PControlFadeState::FADEOUT && tempImage.getImage())
			{
				tempImage.setAlpha(tempAlpha);
				tempImage.draw(ren, tempCamera.getView());
				tempCamera.updateView();

				// Restore the shared texture's alpha.
				tempAlpha = tempImage.getAlpha();
				image.setAlpha(image.getMaxAlpha());
			}
		}
		// Draw temp image when fading in MANUAL mode.
		else if (tempImage.getFadeState() == PControlFadeState::FADEOUT && tempImage.getImage())
			tempImage.draw(ren, tempCamera.getView());

		// Check every frame against the slideshow timer when in slideshow mode.
		if (state == ImageBackgroundState::SLIDESHOW)
			checkSlideshowTimer();

		// Reset rendering target and render the background buffer.
		SDL_SetRenderTarget(ren, nullptr);
		SDL_RenderCopy(ren, background, nullptr, nullptr);
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
	image.pollImageBuffers(ren);
	image.asyncSetImage(path);
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
		if (WindowResizedEvent* windowResizedEvent = dynamic_cast<WindowResizedEvent*>(e))
			OnWindowResized(windowResizedEvent);
		else if (FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e))
		{
			if (PUtils::pathIsImage(fileDropEvent->path))
			{
				enqueueImage(fileDropEvent->path);
				setImage(images.size() - 1);
				e->handled = true;
			}
		}
		else if (ImageLoadReadyEvent* imageLoadReadyEvent = dynamic_cast<ImageLoadReadyEvent*>(e))
		{
			OnImageReady();
			e->handled = true;
		}
		else if (ImageLoadedEvent* imageLoadedEvent = dynamic_cast<ImageLoadedEvent*>(e))
		{
			OnImageLoaded(imageLoadedEvent);
			e->handled = true;
		}
		else  if (KeyDownEvent* keyDownEvent = dynamic_cast<KeyDownEvent*>(e))
			setKeyHeld(keyDownEvent->key);
		else if (KeyUpEvent* keyUpEvent = dynamic_cast<KeyUpEvent*>(e))
		{
			setKeyReleased(keyUpEvent->key);

			if (keyHeld(PVariables::IMGBG_ACCESS_KEY))
			{
				if (keyUpEvent->key == PVariables::IMGBG_PREV_IMG_KEY)
					prevImage();
				else if (keyUpEvent->key == PVariables::IMGBG_NEXT_IMG_KEY)
					nextImage();
			}
		}
	}
}

void ImageBackground::calculateFadeDist(float panSpeed)
{
	int framesToFade = (int) std::round(PConstants::P_FRAMERATE * image.getFadeSpeed());
	fadeDist = (int) std::round(framesToFade * panSpeed);
}

bool ImageBackground::viewInFadeZone(ImageCamera& camera, Image& img)
{
	SDL_Rect view = *camera.getView();

	switch (camera.getPanningState())
	{
		case CameraPanningState::LEFT:
			if (view.x <= fadeDist)
				return true;
			break;
		case CameraPanningState::RIGHT:
			if (view.x + view.w >= iw - fadeDist)
				return true;
			break;
		case CameraPanningState::TOP:
			if (view.y <= fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM:
			if (view.y + view.h >= ih - fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			if (view.y + view.h >= ih - fadeDist || view.x + view.w >= iw - fadeDist)
				return true;
			break;
		case CameraPanningState::BOTTOM_LEFT:
			if (view.y + view.h >= ih - fadeDist || view.x <= fadeDist)
				return true;
			break;
		case CameraPanningState::TOP_RIGHT:
			if (view.y <= fadeDist || view.x + view.w >= iw - fadeDist)
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

void ImageBackground::OnWindowResized(WindowResizedEvent* e)
{
	if (background)
	{
		SDL_DestroyTexture(background);

		background = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, e->ww, e->wh);
		SDL_SetTextureBlendMode(background, SDL_BLENDMODE_BLEND);
	}

	image.setWidth((float) e->ww);
	image.setHeight((float) e->wh);
	tempImage.setWidth((float)e->ww);
	tempImage.setHeight((float)e->wh);
}

void ImageBackground::OnImageReady()
{
	// Initiate fading to the new image, if there exists an image to fade to.
	if (images.size() > 1)
	{
		tempCamera = imageCamera;
		tempImage = image;

		tempImage.setAlpha(tempImage.getMaxAlpha());
		tempImage.setFadeState(PControlFadeState::FADEOUT);
	}

	// Reset slideshow timer count.
	frameCount = 0;
}

void ImageBackground::OnImageLoaded(ImageLoadedEvent* e)
{
	iw = e->iw;
	ih = e->ih;
	imageCamera.setView(e->iw, e->ih);

	// Reset alpha, which is necessary if the previous image did not fully fade out before setImage was called again.
	tempImage.setAlpha(tempImage.getMaxAlpha());
}