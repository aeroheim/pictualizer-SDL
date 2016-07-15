#pragma once

#include "ImageBackground.h"
#include "../util/PUtils.h"
#include "../config/PGlobals.h"

using namespace PGlobals;

ImageBackground::ImageBackground(SDL_Renderer* ren, int ww, int wh) : 
	ren(ren), 
	currentImageCamera(ww, wh), fadingCamera(ww, wh), 
	currentImage(0.0f, 0.0f, static_cast<float>(ww), static_cast<float>(wh)), fadingImage(0.0f, 0.0f, static_cast<float>(ww), static_cast<float>(wh))
{
	// Create the main background buffer where the ImageBackground will be drawn on.
	backgroundBuffer = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, ww, wh);
	SDL_SetTextureBlendMode(backgroundBuffer, SDL_BLENDMODE_BLEND);

	// Configure the images used for the ImageBackground.
	currentImage.setMaxAlpha(PConstants::IMGBG_MAX_ALPHA);
	currentImage.setMinAlpha(PConstants::IMGBG_MIN_ALPHA);
	currentImage.setAlpha(currentImage.getMaxAlpha());
	currentImage.setFadeSpeed(PConstants::IMGBG_ROAM_FADE_SPEED);

	fadingImage.setMaxAlpha(PConstants::IMGBG_MAX_ALPHA);
	fadingImage.setMinAlpha(PConstants::IMGBG_MIN_ALPHA);
	fadingImage.setAlpha(PConstants::IMGBG_MAX_ALPHA);
	fadingImage.setFadeSpeed(PConstants::IMGBG_ROAM_FADE_SPEED);

	// Configure camera and camera-related variables.
	currentImageCamera.setPanSpeed(PConstants::IMGBG_PAN_SPEED);
	currentImageCamera.setState(ImageCameraState::ROAMING);
	calculateFadeDistance(currentImageCamera.getPanSpeed());

	// Event/input handling.
	EventSubscriber::subscribeTo(&currentImage);
	EventObserver::addSubscriber(&currentImageCamera);
	registerKey(PVariables::IMGBG_ACCESS_KEY);
}

ImageBackground::~ImageBackground()
{
	if (backgroundBuffer)
		SDL_DestroyTexture(backgroundBuffer);
}

void ImageBackground::draw()
{
	if (!images.empty())
	{
		// Switch and render to backgroundBuffer.
		SDL_SetRenderTarget(ren, backgroundBuffer);

		switch (currentImageCamera.getState())
		{
			case ImageCameraState::MANUAL:
				drawManualImage();
			case ImageCameraState::ROAMING:
				drawRoamingImage();
		}

		// Check every frame against the slideshow timer when in slideshow mode.
		if (slideshowState == ImageBackgroundState::SLIDESHOW)
			checkSlideshowTimer();

		// Switch back to main buffer and render the backgroundBuffer to it.
		SDL_SetRenderTarget(ren, nullptr);
		SDL_RenderCopy(ren, backgroundBuffer, nullptr, nullptr);
	}
}

void ImageBackground::drawRoamingImage()
{
	// Draw the current image.
	currentImage.draw(ren, currentImageCamera.getView());
	currentImageCamera.updateView();

	// Initialize fade out sequence when the roaming currentImageCamera hits a fade zone.
	if (viewInFadeZone(currentImageCamera, currentImage) && fadingImage.getFadeState() != PControlFadeState::FADEOUT)
	{
		/*
		* Fading out is done by assigning fadingImage to currentImage (so that they share the same texture) and then
		* fading out fadingImage. Each frame during the fade out has currentImage drawn with max alpha and then fadingImage
		* drawn on top using a fadingImageAlpha.
		*
		* Panning two separate views simultaneously while fading out is achieved by using another camera
		* along with the main camera. fadingCamera is assigned to currentImageCamera and continues panning in
		* the same direction, while currentImageCamera resets with a new position and trajectory.
		*/

		fadingCamera = currentImageCamera;
		fadingImage = currentImage;

		fadingImageAlpha = fadingImage.getMaxAlpha();
		fadingImage.setFadeState(PControlFadeState::FADEOUT);

		currentImageCamera.resetPanning();
	}

	// Fade out.
	if (fadingImage.getFadeState() == PControlFadeState::FADEOUT && fadingImage.getImage())
	{
		// Set the texture's alpha to the fade out alpha.
		fadingImage.setAlpha(fadingImageAlpha);
		fadingImage.draw(ren, fadingCamera.getView());

		// Store the new alpha value to use in the next frame.
		fadingImageAlpha = fadingImage.getAlpha();

		// Set the texture's alpha to a value based on the fadingImageAlpha. This allows for a smoother fade-out look.
		currentImage.setAlpha(std::abs(fadingImageAlpha - currentImage.getMaxAlpha()));

		fadingCamera.updateView();
	}
}

void ImageBackground::drawManualImage()
{
	// Draw the current image.
	currentImage.draw(ren, currentImageCamera.getView());

	// Draw the fadingImage in MANUAL mode when switching to another image.
	if (fadingImage.getFadeState() == PControlFadeState::FADEOUT && fadingImage.getImage())
		fadingImage.draw(ren, fadingCamera.getView());
}


void ImageBackground::setState(ImageBackgroundState s)
{
	slideshowState = s;

	if (s == ImageBackgroundState::SLIDESHOW)
		frameCount = 0;
}

void ImageBackground::setImage(std::string path)
{
	currentImage.pollImageBuffers(ren);
	currentImage.asyncSetImage(path);
}

void ImageBackground::setImage(int index)
{
	if (index >= 0 && static_cast<size_t>(index) < images.size())
	{
		setImage(images[index]);
		slideshowIndex = index;
	}
}

void ImageBackground::nextImage()
{
	if (slideshowIndex + 1 >= 0 && static_cast<size_t>(slideshowIndex + 1) < images.size())
	{
		slideshowIndex += 1;
		setImage(images[slideshowIndex]);
	}
}

void ImageBackground::prevImage()
{
	if (slideshowIndex - 1 >= 0 && static_cast<size_t>(slideshowIndex - 1) < images.size())
	{
		slideshowIndex -= 1;
		setImage(images[slideshowIndex]);
	}
}

void ImageBackground::enqueueImage(std::string path)
{
	images.push_back(path);
}

void ImageBackground::removeImage(int index)
{
	if (index >= 0 && static_cast<size_t>(index) < images.size())
	{
		if (slideshowIndex == index)
			setImage(index - 1);
		else if (slideshowIndex < index)
			--slideshowIndex;

		images.erase(images.begin() + index);
	}
}

void ImageBackground::handleEvent(Event* e)
{
	notify(e);

	if (!e->handled)
	{
		if (auto windowResizedEvent = dynamic_cast<WindowResizedEvent*>(e))
			OnWindowResized(windowResizedEvent);
		else if (auto fileDropEvent = dynamic_cast<FileDropEvent*>(e))
		{
			if (PUtils::pathIsImage(fileDropEvent->path))
			{
				enqueueImage(fileDropEvent->path);
				setImage(images.size() - 1);
				e->handled = true;
			}
		}
		else if (auto imageLoadReadyEvent = dynamic_cast<ImageLoadReadyEvent*>(e))
		{
			OnImageReady();
			e->handled = true;
		}
		else if (auto imageLoadedEvent = dynamic_cast<ImageLoadedEvent*>(e))
		{
			OnImageLoaded(imageLoadedEvent);
			e->handled = true;
		}
		else  if (auto keyDownEvent = dynamic_cast<KeyDownEvent*>(e))
			setKeyHeld(keyDownEvent->key);
		else if (auto keyUpEvent = dynamic_cast<KeyUpEvent*>(e))
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

void ImageBackground::calculateFadeDistance(float panSpeed)
{
	int framesToFade = static_cast<int>(std::round(PConstants::P_FRAMERATE * currentImage.getFadeSpeed()));
	fadeDistance = static_cast<int>(std::round(framesToFade * panSpeed));
}

bool ImageBackground::viewInFadeZone(ImageCamera& camera, Image& img) const
{
	SDL_Rect view = *camera.getView();

	switch (camera.getPanningState())
	{
		case CameraPanningState::LEFT:
			if (view.x <= fadeDistance)
				return true;
			break;
		case CameraPanningState::RIGHT:
			if (view.x + view.w >= iw - fadeDistance)
				return true;
			break;
		case CameraPanningState::TOP:
			if (view.y <= fadeDistance)
				return true;
			break;
		case CameraPanningState::BOTTOM:
			if (view.y + view.h >= ih - fadeDistance)
				return true;
			break;
		case CameraPanningState::BOTTOM_RIGHT:
			if (view.y + view.h >= ih - fadeDistance || view.x + view.w >= iw - fadeDistance)
				return true;
			break;
		case CameraPanningState::BOTTOM_LEFT:
			if (view.y + view.h >= ih - fadeDistance || view.x <= fadeDistance)
				return true;
			break;
		case CameraPanningState::TOP_RIGHT:
			if (view.y <= fadeDistance || view.x + view.w >= iw - fadeDistance)
				return true;
			break;
		case CameraPanningState::TOP_LEFT:
			if (view.y <= fadeDistance || view.x <= fadeDistance)
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
		if ((static_cast<float>(frameCount) / 60) == slideshowTimer)
		{
			if (static_cast<size_t>(slideshowIndex + 1) < images.size())
				nextImage();
			else
				setImage(0);
		}
	}
}

void ImageBackground::OnWindowResized(WindowResizedEvent* e)
{
	if (backgroundBuffer)
	{
		SDL_DestroyTexture(backgroundBuffer);

		backgroundBuffer = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, e->ww, e->wh);
		SDL_SetTextureBlendMode(backgroundBuffer, SDL_BLENDMODE_BLEND);
	}

	currentImage.setWidth(static_cast<float>(e->ww));
	currentImage.setHeight(static_cast<float>(e->wh));
	fadingImage.setWidth(static_cast<float>(e->ww));
	fadingImage.setHeight(static_cast<float>(e->wh));
}

void ImageBackground::OnImageReady()
{
	// Initiate fading to the new image, if there exists an image to fade to.
	if (images.size() > 1)
	{
		fadingCamera = currentImageCamera;
		fadingImage = currentImage;

		fadingImageAlpha = fadingImage.getMaxAlpha();
		fadingImage.setAlpha(fadingImage.getMaxAlpha());
		fadingImage.setFadeState(PControlFadeState::FADEOUT);
	}

	// Reset slideshow timer count.
	frameCount = 0;
}

void ImageBackground::OnImageLoaded(ImageLoadedEvent* e)
{
	iw = e->iw;
	ih = e->ih;
	currentImageCamera.setView(e->iw, e->ih);

	// Reset alpha, which is necessary if the previous image did not fully fade out before setImage was called again.
	fadingImage.setAlpha(fadingImage.getMaxAlpha());
}