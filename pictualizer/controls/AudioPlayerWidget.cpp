#pragma once

#include "AudioPlayerWidget.h"
#include <sstream>
#include "../assets/fonts/PFonts.h"
#include "../assets/textures/PTextures.h"
#include "../util/PUtils.h"

using namespace std;

AudioPlayerWidget::AudioPlayerWidget(SDL_Renderer* ren, AudioPlayer* audioPlayer, float x, float y, float w, float h) :
	PWidget(ren, x, y, w, h),
	displayState(AudioPlayerWidgetDisplayState::PLAYLIST_NUMBER),
	visualizationState(AudioPlayerWidgetVisualizationState::SPECTRUM),
	ren(ren),
	audioPlayer(audioPlayer),
	playerControlGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() / 2, getInnerHeight() / 2 }, std::vector < float > { getInnerHeight() / 2, getInnerHeight() / 2 }),
	bodyGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() }, std::vector < float > { getInnerHeight(), getInnerWidth() - getInnerHeight() }),
	rightGrid(getInnerX() + getInnerHeight(), getInnerY(), std::vector < float > { getInnerHeight() * (5.0f / 18), getInnerHeight() * (3.0f / 18), getInnerHeight() * (8.0f / 18), getInnerHeight() * (2.0f / 18) }, std::vector < float > { getInnerWidth() - getInnerHeight() }),
	bottomGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() * (2.0f / 18) }, std::vector < float > { getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), (getInnerWidth() - getInnerHeight()) - ((getInnerHeight() * (2.0f / 18)) * 7.5f), getInnerHeight() * (2.0f / 18), 2.5f * (getInnerHeight() * (2.0f / 18)) }),
	volButtonGrid(getInnerX(), getInnerY(), std::vector < float > { 100, 100 }, std::vector < float > { 200 }),
	leftControlsContainer(0, 0, 100, 100),
	visualizationContainer(0, 0, 100, 100),
	indexWidget(ren, PFontType::CENTURYGOTHIC, 0, 0, 100, 100),
	albumArt(0, 0, 100, 100),
	album(L""),
	seekBar(ren, PFontType::MPLUSLIGHT, 0, 0, 64, 64),
	title(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100),
	artist(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100),
	volDb(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100),
	playPause(PTextureType::AP_PLAY, 0, 0, 64, 64),
	stop(PTextureType::AP_STOP, 0, 0, 64, 64),
	prevTrack(PTextureType::AP_LEFT, 0, 0, 64, 64),
	nextTrack(PTextureType::AP_RIGHT, 0, 0, 64, 64),
	repeat(PTextureType::AP_REPEAT, 0, 0, 64, 64),
	shuffle(PTextureType::AP_SHUFFLE, 0, 0, 64, 64),
	playlist(PTextureType::AP_PLAYLIST, 0, 0, 64, 64),
	info(PTextureType::AP_INFO, 0, 0, 64, 64),
	volUp(PTextureType::AP_VOL_UP, 0, 0, 64, 32),
	volDown(PTextureType::AP_VOL_DOWN, 0, 0, 64, 32),
	waveformVisualizer(0, 0, 200, 100),
	spectrumVisualizer(0, 0, 200, 200),
	frameCount(1),
	seeking(false)
{
	// AudioPlayer control buttons - PLAY, PAUSE, STOP, PREV, NEXT.
	playPause.setBaseColor(180, 180, 180);
	playPause.setColorSpeed(0.225f);

	stop.setBaseColor(180, 180, 180);
	stop.setColorSpeed(0.225f);

	prevTrack.setBaseColor(180, 180, 180);
	prevTrack.setColorSpeed(0.225f);

	nextTrack.setBaseColor(180, 180, 180);
	nextTrack.setColorSpeed(0.225f);

	// IndexWidget - supports track numbers from 0 to 999,999,999.
	indexWidget.setIndex(0);
	indexWidget.setMinAlpha(0);
	indexWidget.setMaxAlpha(255);
	indexWidget.setFadeSpeed(0.225f);

	// Album Art - displays album art from current track.
	albumArt.setAlpha(0);
	albumArt.setMinAlpha(0);
	albumArt.setMaxAlpha(255);
	albumArt.setFadeSpeed(0.225f);
	albumArt.setX(bodyGrid[0][0].getX());
	albumArt.setY(bodyGrid[0][0].getY());
	albumArt.setWidth(bodyGrid[0][0].getWidth());
	albumArt.setHeight(bodyGrid[0][0].getHeight());

	// Default album art.
	defaultAlbumArt = Image::makeImageTexture(getDefaultAlbumArt());
	albumArt.setImage(defaultAlbumArt);

	// Metadata labels - TITLE, ARTIST.
	title.setClipState(LabelClipState::PAN);
	title.setText("Pictualizer");
	artist.setClipState(LabelClipState::PAN);
	artist.setText("artist");

	// Playlist buttons - PLAYLIST, INFO, REPEAT, SHUFFLE.
	repeat.setButtonStyle(ButtonStyle::TOGGLE);
	repeat.setBaseColor(180, 180, 180);
	repeat.setColorSpeed(0.25f);

	shuffle.setButtonStyle(ButtonStyle::TOGGLE);
	shuffle.setBaseColor(180, 180, 180);
	shuffle.setColorSpeed(0.25f);

	playlist.setBaseColor(180, 180, 180);
	playlist.setColorSpeed(0.25);

	info.setBaseColor(180, 180, 180);
	info.setColorSpeed(0.25f);
	
	// SeekBar - linked to AudioPlayer.
	seekBar.setBarColor(100, 100, 100);
	seekBar.setTime(0);
	seekBar.setDuration(0);

	// Misc. controls - VOL buttons & labels.
	volUp.setBaseColor(180, 180, 180);
	volUp.setColorSpeed(0.25f);

	volDown.setBaseColor(180, 180, 180);
	volDown.setColorSpeed(0.25f);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << audioPlayer->getVolume();

	volDb.setClipState(LabelClipState::CLIP);
	volDb.setText(ss.str() + "dB");

	// Visualizations - WAVEFORM & SPECTRUM visualizers.
	// waveformVisualizer.setMsOffset(150);
	waveformVisualizer.setMsOffset(330);
	waveformVisualizer.setAlpha(0);
	waveformVisualizer.setMinAlpha(0);
	waveformVisualizer.setMaxAlpha(230);
	waveformVisualizer.setFadeSpeed(0.25f);

	spectrumVisualizer.addBin(0, 300, 0.5);
	spectrumVisualizer.addBin(300, 900, 1.5);
	spectrumVisualizer.addBin(900, 1500, 1.5);
	spectrumVisualizer.setBarWidth(2);
	spectrumVisualizer.setDividerWidth(15);
	spectrumVisualizer.setMinAlpha(0);
	spectrumVisualizer.setMaxAlpha(230);
	spectrumVisualizer.setFadeSpeed(0.25f);

	// Containers.
	visualizationContainer.addElement(&spectrumVisualizer);
	visualizationContainer.addElement(&waveformVisualizer);
	visualizationContainer.setDrawElement(0);
	visualizationContainer.setTransitionState(PControlContainerTransitionState::FADE);

	leftControlsContainer.addElement(&indexWidget);
	leftControlsContainer.addElement(&albumArt);
	leftControlsContainer.setDrawElement(0);
	leftControlsContainer.setTransitionState(PControlContainerTransitionState::FADE);

	// Grids.
	playerControlGrid[0][0].setElement(&playPause);
	playerControlGrid[0][0].setPadding(0.5f, 0.5f, 0, 0);
	playerControlGrid[0][1].setElement(&stop);
	playerControlGrid[0][1].setPadding(0, 0.5f, 0.5f, 0);
	playerControlGrid[1][0].setElement(&prevTrack);
	playerControlGrid[1][0].setPadding(0.5f, 0, 0.05f, 0.5f);
	playerControlGrid[1][1].setElement(&nextTrack);
	playerControlGrid[1][1].setPadding(0.05f, 0, 0.5f, 0.5f);
	playerControlGrid.setAlpha(0);
	playerControlGrid.setMinAlpha(0);
	playerControlGrid.setMaxAlpha(255);
	playerControlGrid.setFadeSpeed(0.225f);

	bodyGrid[0][0].setElement(&leftControlsContainer);
	leftControlsContainer.addElement(&playerControlGrid);

	rightGrid[0][0].setElement(&title);
	rightGrid[1][0].setElement(&artist);
	rightGrid[2][0].setElement(&visualizationContainer);
	rightGrid[2][0].setPadding(0, 0.1f, 0, 0.1f);
	rightGrid[3][0].setElement(&bottomGrid);

	bottomGrid[0][0].setElement(&playlist);
	bottomGrid[0][1].setElement(&info);
	bottomGrid[0][2].setElement(&repeat);
	bottomGrid[0][2].setPadding(0.15f, 0.14f, 0.15f, 0.16f);
	bottomGrid[0][3].setElement(&shuffle);
	bottomGrid[0][3].setPadding(0.1f, 0.1f, 0.1f, 0.1f);
	bottomGrid[0][4].setElement(&seekBar);
	bottomGrid[0][4].setPadding(0.02f, 0, 0, 0);

	volButtonGrid[0][0].setElement(&volUp);
	volButtonGrid[0][0].setPadding(0, 0.2f, 0, 0);
	volButtonGrid[1][0].setElement(&volDown);
	volButtonGrid[1][0].setPadding(0, 0, 0, 0.2f);

	bottomGrid[0][5].setElement(&volButtonGrid);
	bottomGrid[0][5].setPadding(0, 0.08f, 0, 0);
	bottomGrid[0][6].setElement(&volDb);
	bottomGrid[0][6].setPadding(0, 0.08f, 0, 0);

	bodyGrid[0][1].setElement(&rightGrid);
	bodyGrid[0][1].setPadding(0.05f, 0, 0.02f, 0);

	// PWidget background settings.
	setBackgroundMinAlpha(150);
	setBackgroundAlpha(0);
	setBackgroundMaxAlpha(200);
	setBackgroundFadeSpeed(0.225f);
	setBackgroundColor(0, 0, 0);

	// CUHRAYZEE COLORS!!!
	/*
	waveformVisualizer.setColor(0, 0, 0);
	spectrumVisualizer.setColor(30, 250, 96);
	title.setColor(69, 130, 250);
	artist.setColor(150, 50, 150);
	volDb.setColor(69, 69, 150);
	setBackgroundColor(215, 73, 0);
	*/
	
	// Event subscribing.
	addSubscriber(&playPause);
	addSubscriber(&stop);
	addSubscriber(&prevTrack);
	addSubscriber(&nextTrack);
	addSubscriber(&repeat);
	addSubscriber(&shuffle);
	addSubscriber(&playlist);
	addSubscriber(&info);
	addSubscriber(&volUp);
	addSubscriber(&volDown);
	addSubscriber(&seekBar);

	subscribeTo(&playPause);
	subscribeTo(&stop);
	subscribeTo(&prevTrack);
	subscribeTo(&nextTrack);
	subscribeTo(&repeat);
	subscribeTo(&shuffle);
	subscribeTo(&playlist);
	subscribeTo(&info);
	subscribeTo(&volUp);
	subscribeTo(&volDown);
	subscribeTo(&seekBar);

	subscribeTo(audioPlayer);
}

AudioPlayerWidget::~AudioPlayerWidget() {};

void AudioPlayerWidget::setX(float x)
{
	PWidget::setX(x);
	bodyGrid.setX(getInnerX());
}

void AudioPlayerWidget::setY(float y)
{
	PWidget::setY(y);
	bodyGrid.setY(getInnerY());
}

void AudioPlayerWidget::setWidth(float w)
{
	PWidget::setWidth(w);
	bodyGrid.setWidth(getInnerWidth());
}

void AudioPlayerWidget::setHeight(float h)
{
	PWidget::setHeight(h);
	bodyGrid.setHeight(getInnerHeight());
}

void AudioPlayerWidget::draw(SDL_Renderer* ren)
{
	PWidget::draw(this->ren);

	// Poll the AudioPlayer for position in current track every second.
	if (audioPlayer->getPlayerState() == AudioPlayerState::PLAYING && !seeking && frameCount % 60 == 0)
	{
		frameCount = 1;
		seekBar.setTime((int) std::floor(audioPlayer->getPosition()));
	}
	
	if (ren)
		bodyGrid.draw(ren);
	else
		bodyGrid.draw(this->ren);

	// TODO: reset this variable every second.
	++frameCount;
}

void AudioPlayerWidget::handleEvent(Event* e)
{
	if (!e->handled)
	{
		if (FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e))
		{
			if (mouseInside(fileDropEvent->x, fileDropEvent->y))
			{
				if (PUtils::pathIsMusic(fileDropEvent->path))
				{
					OnFileDrop(fileDropEvent);
					e->handled = true;
				}
				else if (fileDropEvent->dir)
				{
					OnDirectoryDrop(fileDropEvent);
					e->handled = true;
				}
			}
		}
		else if (ButtonPressedEvent* buttonPressedEvent = dynamic_cast<ButtonPressedEvent*>(e))
		{
			OnButtonPressed(buttonPressedEvent);
			e->handled = true;
		}
		else if (ButtonToggledEvent* buttonToggledEvent = dynamic_cast<ButtonToggledEvent*>(e))
		{
			OnButtonToggled(buttonToggledEvent);
			e->handled = true;
		}
		else if (SeekEvent* seekEvent = dynamic_cast<SeekEvent*>(e))
		{
			OnSeek(seekEvent);
			e->handled = true;
		}
		else if (SeekRequestEvent* seekRequestEvent = dynamic_cast<SeekRequestEvent*>(e))
		{
			OnSeekRequest(seekRequestEvent);
			e->handled = true;
		}
		else if (NewTrackEvent* newTrackEvent = dynamic_cast<NewTrackEvent*>(e))
		{
			OnNewTrack(newTrackEvent);
			e->handled = true;
		}
		else if (PlayerStartedEvent* playerStartedEvent = dynamic_cast<PlayerStartedEvent*>(e))
		{
			OnPlayerStarted(playerStartedEvent);
			e->handled = true;
		}
		else if (PlayerPausedEvent* playerPausedEvent = dynamic_cast<PlayerPausedEvent*>(e))
		{
			OnPlayerPaused(playerPausedEvent);
			e->handled = true;
		}
		else if (PlayerStoppedEvent* playerStoppedEvent = dynamic_cast<PlayerStoppedEvent*>(e))
		{
			OnPlayerStopped(playerStoppedEvent);
			e->handled = true;
		}
		else if (VolumeChangedEvent* volumeChangedEvent = dynamic_cast<VolumeChangedEvent*>(e))
		{
			OnVolumeChanged(volumeChangedEvent);
			e->handled = true;
		}
		else if (WindowResizedEvent* windowResizedEvent = dynamic_cast<WindowResizedEvent*>(e))
			OnWindowResized(windowResizedEvent);
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
			OnMouseMotion(mouseMotionEvent);
		else if (MouseUpEvent* mouseUpEvent = dynamic_cast<MouseUpEvent*>(e))
			OnMouseUp(mouseUpEvent);


		PWidget::handleEvent(e);
	}
}

void AudioPlayerWidget::enqueueTrack(const std::string& path)
{
	AudioTrack newTrack(path);

	AudioPlaylist* currentPlaylist = audioPlayer->getCurrentPlaylist();
	currentPlaylist->enqueueTrack(newTrack);
}

SDL_Texture* AudioPlayerWidget::getDefaultAlbumArt()
{
	// Create the default album art background.
	SDL_Surface* aaSurface = SDL_CreateRGBSurface(0, 600, 600, 32, 0, 0, 0, 0);
	SDL_FillRect(aaSurface, NULL, SDL_MapRGBA(aaSurface->format, 85, 85, 85, 85));
	SDL_Texture* aaBg = SDL_CreateTextureFromSurface(ren, aaSurface);
	SDL_FreeSurface(aaSurface);

	// Create the "?" text on the default album art.
	Label aaLabel(ren, PFontType::CENTURYGOTHIC, 0, 0, 600, 600);
	aaLabel.setAlignState(LabelAlignState::CENTER);
	aaLabel.setText("?");
	
	// Create the default album art texture.
	SDL_Texture* aaTexture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 600, 600);
	SDL_SetTextureBlendMode(aaTexture, SDL_BLENDMODE_BLEND);

	// Render to the default album art.bv
	SDL_SetRenderTarget(ren, aaTexture);
	SDL_RenderCopy(ren, aaBg, nullptr, nullptr);
	aaLabel.draw(ren);
	SDL_SetRenderTarget(ren, nullptr);

	// Clean-up.
	SDL_DestroyTexture(aaBg);

	return aaTexture;
}

void AudioPlayerWidget::OnFileDrop(FileDropEvent* e)
{
	enqueueTrack(e->path);
}

void AudioPlayerWidget::OnDirectoryDrop(FileDropEvent* e)
{
	for (auto it = boost::filesystem::recursive_directory_iterator(PUtils::str2wstr(e->path)); it != boost::filesystem::recursive_directory_iterator(); it++)
	{
		const auto& file = it->path();

		if (PUtils::pathIsMusic(file.filename()))
			enqueueTrack(file.string());
	}
}

void AudioPlayerWidget::OnButtonPressed(ButtonPressedEvent* e)
{
	if (e->button == &playPause)
	{
		if (audioPlayer->getPlayerState() == AudioPlayerState::PLAYING)
			audioPlayer->pause();
		else
			audioPlayer->play();
	}
	else if (e->button == &stop)
		audioPlayer->stop();
	else if (e->button == &prevTrack)
		audioPlayer->prevTrack();
	else if (e->button == &nextTrack)
		audioPlayer->nextTrack();
	else if (e->button == &volUp)
		audioPlayer->setVolume(audioPlayer->getVolume() + VOL_DELTA);
	else if (e->button == &volDown)
		audioPlayer->setVolume(audioPlayer->getVolume() - VOL_DELTA);

	/*
	else if (e->button == &playlist)
		// bring out playlist widget
	else if (e->button == &info)
		// bring out info widget
	*/
}

void AudioPlayerWidget::OnMouseMotion(MouseMotionEvent* e)
{
	if (leftControlsContainer.mouseInside(e->x, e->y))
	{
		// Fade to player controls.
		if (displayState == AudioPlayerWidgetDisplayState::ALBUM_ART && albumArt.getMinAlpha() != ALBUM_ART_MIN_ALPHA)
			albumArt.setMinAlpha(ALBUM_ART_MIN_ALPHA);

		if (leftControlsContainer.getElementIndex() != 2)
			leftControlsContainer.setDrawElement(2);
	}
	else 
	{
		// Fade to album art.
		if (displayState == AudioPlayerWidgetDisplayState::ALBUM_ART && leftControlsContainer.getElementIndex() != 1)
		{
			if (albumArt.getMaxAlpha() == ALBUM_ART_MIN_ALPHA)
				albumArt.setMaxAlpha(255);

			leftControlsContainer.setDrawElement(1);
		}
		// Fade to playlist number.
		else if (displayState == AudioPlayerWidgetDisplayState::PLAYLIST_NUMBER && leftControlsContainer.getElementIndex() != 0)
		{
			if (albumArt.getMinAlpha() != 0)
				albumArt.setMinAlpha(0);

			leftControlsContainer.setDrawElement(0);
		}

		if (playerControlGrid.getMinAlpha() == 255)
			playerControlGrid.setMinAlpha(0);
	}
}

void AudioPlayerWidget::OnMouseUp(MouseUpEvent* e)
{
	if (leftControlsContainer.mouseInside(e->x, e->y) && e->button == SDL_BUTTON_RIGHT)
	{
		if (displayState == AudioPlayerWidgetDisplayState::PLAYLIST_NUMBER)
		{
			// Change to album art.
			displayState = AudioPlayerWidgetDisplayState::ALBUM_ART;

			albumArt.setMinAlpha(0);
			albumArt.setMaxAlpha(ALBUM_ART_MIN_ALPHA);
			
			playerControlGrid.setMinAlpha(255);

			leftControlsContainer.setDrawElement(1);
		}
		else if (displayState == AudioPlayerWidgetDisplayState::ALBUM_ART)
		{
			// Change to playlist number.
			displayState = AudioPlayerWidgetDisplayState::PLAYLIST_NUMBER;

			albumArt.setMinAlpha(0);
			albumArt.setFadeState(PControlFadeState::FADEOUT);
		}
	}

	if (visualizationContainer.mouseInside(e->x, e->y) && e->button == SDL_BUTTON_RIGHT)
	{
		if (visualizationState == AudioPlayerWidgetVisualizationState::SPECTRUM)
		{
			visualizationContainer.setDrawElement(1);
			visualizationState = AudioPlayerWidgetVisualizationState::WAVEFORM;
		}
		else
		{
			visualizationContainer.setDrawElement(0);
			visualizationState = AudioPlayerWidgetVisualizationState::SPECTRUM;
		}
	}
}

void AudioPlayerWidget::OnWindowResized(WindowResizedEvent* e)
{
	if (defaultAlbumArt == albumArt.getImage())
	{
		defaultAlbumArt = Image::makeImageTexture(getDefaultAlbumArt());
		albumArt.setImage(defaultAlbumArt);
	}
	else
		defaultAlbumArt = Image::makeImageTexture(getDefaultAlbumArt());
}

void AudioPlayerWidget::OnButtonToggled(ButtonToggledEvent* e)
{
	if (e->button == &repeat)
	{
		if (audioPlayer->getRepeat() == RepeatState::SONG)
			audioPlayer->setRepeat(RepeatState::NONE);
		else
			audioPlayer->setRepeat(RepeatState::SONG);
	}
	else if (e->button == &shuffle)
	{
		if (audioPlayer->getShuffle() == ShuffleState::PLAYLIST)
			audioPlayer->setShuffle(ShuffleState::NONE);
		else
			audioPlayer->setShuffle(ShuffleState::PLAYLIST);
	}
}

void AudioPlayerWidget::OnSeek(SeekEvent* e)
{
	seeking = true;
}

void AudioPlayerWidget::OnSeekRequest(SeekRequestEvent* e)
{
	seeking = false;
	
	int position = e->seconds > audioPlayer->getBASSDuration() ? (int) audioPlayer->getBASSDuration() : e->seconds;
	audioPlayer->setPosition(position);

	frameCount = 1;
}

void AudioPlayerWidget::OnNewTrack(NewTrackEvent * e)
{
	title.setText(e->track->getTitle());
	artist.setText(e->track->getArtist());

	// Fetch a new album art if the album has changed.
	if (album.compare(e->track->getAlbum()) != 0)
	{
	 	album = e->track->getAlbum();
		albumArt.pollImageBuffers(ren);

		ImageRWops& imageRWops = e->track->getAlbumArt(ren);

		if (imageRWops.rwops)
			albumArt.asyncSetImage(imageRWops);
		else
			albumArt.setImage(defaultAlbumArt);
	}

	indexWidget.setIndex(audioPlayer->getCurrentTrackIndex() + 1);
	seekBar.setTime(0);
	seekBar.setDuration((int) std::floor(audioPlayer->getBASSDuration()));
	waveformVisualizer.setStream(audioPlayer->getStream());
	spectrumVisualizer.setStream(audioPlayer->getStream());
}

void AudioPlayerWidget::OnPlayerStarted(PlayerStartedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PAUSE);

	if (visualizationState == AudioPlayerWidgetVisualizationState::WAVEFORM)
		waveformVisualizer.setFadeState(PControlFadeState::FADEIN);
}

void AudioPlayerWidget::OnPlayerPaused(PlayerPausedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PLAY);

	if (visualizationState == AudioPlayerWidgetVisualizationState::WAVEFORM)
		waveformVisualizer.setFadeState(PControlFadeState::FADEOUT);
}

void AudioPlayerWidget::OnPlayerStopped(PlayerStoppedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PLAY);

	if (visualizationState == AudioPlayerWidgetVisualizationState::WAVEFORM)
		waveformVisualizer.setFadeState(PControlFadeState::FADEOUT);

	seekBar.setTime(0);
}

void AudioPlayerWidget::OnVolumeChanged(VolumeChangedEvent* e)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << audioPlayer->getVolume();

	volDb.setText(ss.str() + "dB");
}
