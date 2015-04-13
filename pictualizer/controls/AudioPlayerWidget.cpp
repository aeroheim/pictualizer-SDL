#include "AudioPlayerWidget.h"

AudioPlayerWidget::AudioPlayerWidget(SDL_Renderer* ren, AudioPlayer* audioPlayer, float x, float y, float w, float h) :
	PWidget(ren, x, y, w, h),
	ren(ren),
	audioPlayer(audioPlayer),
	playerControlGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() / 2, getInnerHeight() / 2 }, std::vector < float > { getInnerHeight() / 2, getInnerHeight() / 2 }),
	bodyGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() }, std::vector < float > { getInnerHeight(), getInnerWidth() - getInnerHeight() }),
	rightGrid(getInnerX() + getInnerHeight(), getInnerY(), std::vector < float > { getInnerHeight() * (5.0f / 18), getInnerHeight() * (3.0f / 18), getInnerHeight() * (8.0f / 18), getInnerHeight() * (2.0f / 18) }, std::vector < float > { getInnerWidth() - getInnerHeight() }),
	bottomGrid(getInnerX(), getInnerY(), std::vector < float > { getInnerHeight() * (2.0f / 18) }, std::vector < float > { getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), (getInnerWidth() - getInnerHeight()) - ((getInnerHeight() * (2.0f / 18)) * 7.5f), getInnerHeight() * (2.0f / 18), 2.5f * (getInnerHeight() * (2.0f / 18)) }),
	volButtonGrid(getInnerX(), getInnerY(), std::vector < float > { 100, 100 }, std::vector < float > { 200 }),
	indexWidget(ren, PFontType::CENTURYGOTHIC, 0, 0, 100, 100),
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
	waveformVisualizer(ren, 0, 0, 200, 100),
	seeking(false)
{

	// AudioPlayer control buttons - PLAY, PAUSE, STOP, PREV, NEXT.
	playPause.setBaseTint(160);
	playPause.setTintDelta(7);
	addSubscriber(&playPause);

	stop.setBaseTint(160);
	stop.setTintDelta(7);
	addSubscriber(&stop);

	prevTrack.setBaseTint(160);
	prevTrack.setTintDelta(7);
	addSubscriber(&prevTrack);

	nextTrack.setBaseTint(160);
	nextTrack.setTintDelta(7);
	addSubscriber(&nextTrack);

	// IndexWidget - supports track numbers from 0 to 999,999,999.
	indexWidget.setIndex(0);

	// Metadata labels - TITLE, ARTIST.
	title.setClipState(LabelClipState::PAN);
	title.setText("Pictualizer");
	artist.setClipState(LabelClipState::PAN);
	artist.setText("artist");

	// Playlist buttons - PLAYLIST, INFO, REPEAT, SHUFFLE.
	repeat.setButtonStyle(ButtonStyle::TOGGLE);
	repeat.setBaseTint(160);
	repeat.setTintDelta(7);
	addSubscriber(&repeat);

	shuffle.setButtonStyle(ButtonStyle::TOGGLE);
	shuffle.setBaseTint(160);
	shuffle.setTintDelta(7);
	addSubscriber(&shuffle);

	playlist.setBaseTint(160);
	playlist.setTintDelta(7);
	addSubscriber(&playlist);

	info.setBaseTint(160);
	info.setTintDelta(7);
	addSubscriber(&info);
	
	// SeekBar - linked to AudioPlayer.
	seekBar.setBaseColor(100, 100, 100);
	seekBar.setTime(0);
	seekBar.setDuration(0);
	addSubscriber(&seekBar);

	// Misc. controls - VOL buttons & labels.
	volUp.setBaseTint(160);
	volUp.setTintDelta(7);
	addSubscriber(&volUp);

	volDown.setBaseTint(160);
	volDown.setTintDelta(7);
	addSubscriber(&volDown);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << BASS_GetVolume();

	volDb.setClipState(LabelClipState::CLIP);
	volDb.setText(ss.str() + "dB");

	// Visualizations - WAVEFORM & SPECTRUM visualizers.
	waveformVisualizer.setMsOffset(330);
	waveformVisualizer.setAlpha(0);
	waveformVisualizer.setMinAlpha(0);
	waveformVisualizer.setMaxAlpha(255);
	waveformVisualizer.setFadeDelta(30);


	// Grids.
	playerControlGrid[0][0].setElement(&playPause);
	playerControlGrid[0][0].setPadding(0.3f, 0.3f, 0, 0);
	playerControlGrid[0][1].setElement(&stop);
	playerControlGrid[0][1].setPadding(0, 0.3f, 0.3f, 0);
	playerControlGrid[1][0].setElement(&prevTrack);
	playerControlGrid[1][0].setPadding(0.3f, 0, 0.05f, 0.3f);
	playerControlGrid[1][1].setElement(&nextTrack);
	playerControlGrid[1][1].setPadding(0.05f, 0, 0.3f, 0.3f);
	playerControlGrid.setAlpha(0);
	playerControlGrid.setMinAlpha(0);
	playerControlGrid.setMaxAlpha(255);
	playerControlGrid.setFadeDelta(15);

	bodyGrid[0][0].setElement(&indexWidget);
	bodyGrid[0][0].setMinAlpha(0);
	bodyGrid[0][0].setMaxAlpha(255);
	bodyGrid[0][0].setFadeDelta(15);

	rightGrid[0][0].setElement(&title);
	rightGrid[1][0].setElement(&artist);
	rightGrid[2][0].setElement(&waveformVisualizer);
	rightGrid[2][0].setPadding(0, 0.05f, 0, 0.05f);
	rightGrid[3][0].setElement(&bottomGrid);

	bottomGrid[0][0].setElement(&playlist);
	bottomGrid[0][1].setElement(&info);
	bottomGrid[0][2].setElement(&repeat);
	bottomGrid[0][2].setPadding(0.15f, 0.15f, 0.15f, 0.15f);
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
	bodyGrid[0][1].setPadding(0.03f, 0, 0, 0);

	// PWidget background settings.
	setBackgroundMinAlpha(150);
	setBackgroundAlpha(0);
	setBackgroundMaxAlpha(200);
	setBackgroundFadeDelta(4.5);
	
	// Event subscribing.
	subscribeTo(&playPause);
	subscribeTo(&stop);
	subscribeTo(&prevTrack);
	subscribeTo(&nextTrack);
	subscribeTo(&playlist);
	subscribeTo(&info);
	subscribeTo(&repeat);
	subscribeTo(&shuffle);
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
	playerControlGrid.setX(getInnerX());
}

void AudioPlayerWidget::setY(float y)
{
	PWidget::setY(y);
	bodyGrid.setY(getInnerY());
	playerControlGrid.setY(getInnerY());
}

void AudioPlayerWidget::setWidth(float w)
{
	PWidget::setWidth(w);
	bodyGrid.setWidth(getInnerWidth());
	playerControlGrid.setWidth(bodyGrid[0][0].getWidth());
}

void AudioPlayerWidget::setHeight(float h)
{
	PWidget::setHeight(h);
	bodyGrid.setHeight(getInnerHeight());
	playerControlGrid.setHeight(bodyGrid[0][0].getHeight());
}

void AudioPlayerWidget::draw(SDL_Renderer* ren)
{
	PWidget::draw(this->ren);

	// Poll the AudioPlayer for position in current track;
	if (audioPlayer->getPlayerState() == AudioPlayerState::PLAYING && !seeking)
		seekBar.setTime((int) audioPlayer->getPosition());

	if (ren)
	{
		bodyGrid.draw(ren);
		playerControlGrid.draw(ren);
	}
	else
	{
		bodyGrid.draw(this->ren);
		playerControlGrid.draw(this->ren);
	}
}

void AudioPlayerWidget::handleEvent(Event* e)
{
	if (!e->handled)
	{
		if (FileDropEvent* fileDropEvent = dynamic_cast<FileDropEvent*>(e))
		{
			if (mouseInside(fileDropEvent->x, fileDropEvent->y) && PUtils::pathIsMusic(fileDropEvent->path))
			{
				OnFileDrop(fileDropEvent);
				e->handled = true;
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
		else if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
		{
			if (playerControlGrid.mouseInside(mouseMotionEvent->x, mouseMotionEvent->y))
			{
				playerControlGrid.setFadeState(PControlFadeState::FADEIN);
				bodyGrid[0][0].setFadeState(PControlFadeState::FADEOUT);
			}
			else
			{
				if (playerControlGrid.getAlpha() != 0)
					playerControlGrid.setFadeState(PControlFadeState::FADEOUT);

				if (bodyGrid[0][0].getAlpha() != 255)
					bodyGrid[0][0].setFadeState(PControlFadeState::FADEIN);
			}
		}

		PWidget::handleEvent(e);
	}
}

void AudioPlayerWidget::OnFileDrop(FileDropEvent* e)
{
	AudioTrack newTrack(e->path);

	AudioPlaylist* currentPlaylist = audioPlayer->getCurrentPlaylist();
	currentPlaylist->enqueueTrack(newTrack);
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
	{
		float newVol = BASS_GetVolume() + VOL_DELTA <= 1 ? BASS_GetVolume() + VOL_DELTA : 1;
		BASS_SetVolume(newVol);

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << BASS_GetVolume();

		volDb.setText(ss.str() + "dB");
	}
	else if (e->button == &volDown)
	{
		float newVol = BASS_GetVolume() - VOL_DELTA >= 0 ? BASS_GetVolume() - VOL_DELTA : 0;
		BASS_SetVolume(newVol);

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << BASS_GetVolume();

		volDb.setText(ss.str() + "dB");
	}

	/*
	else if (e->button == &playlist)
		// bring out playlist widget
	else if (e->button == &info)
		// bring out info widget
	*/
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
}

void AudioPlayerWidget::OnNewTrack(NewTrackEvent * e)
{
	title.setText(e->track->getTitle());
	artist.setText(e->track->getArtist());
	indexWidget.setIndex(audioPlayer->getCurrentTrackIndex() + 1);
	seekBar.setTime(0);
	seekBar.setDuration(e->track->getDuration());
	waveformVisualizer.setStream(audioPlayer->getStream());
}

void AudioPlayerWidget::OnPlayerStarted(PlayerStartedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PAUSE);
	waveformVisualizer.setFadeState(PControlFadeState::FADEIN);
}

void AudioPlayerWidget::OnPlayerPaused(PlayerPausedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PLAY);
	waveformVisualizer.setFadeState(PControlFadeState::FADEOUT);
}

void AudioPlayerWidget::OnPlayerStopped(PlayerStoppedEvent* e)
{
	playPause.setTexture(PTextureType::AP_PLAY);
	waveformVisualizer.setFadeState(PControlFadeState::FADEOUT);
	seekBar.setTime(0);
}
