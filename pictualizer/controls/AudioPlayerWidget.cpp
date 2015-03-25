#include "AudioPlayerWidget.h"
#include <iostream>

using namespace std;

AudioPlayerWidget::AudioPlayerWidget(SDL_Renderer* ren, float x, float y, float w, float h) : 
	PWidget(ren, x, y, w, h),
	playerControlGrid(getInnerX(), getInnerY(), std::vector<float> { getInnerHeight() / 2, getInnerHeight() / 2 }, std::vector<float> { getInnerHeight() / 2, getInnerHeight() / 2 }),
	bodyGrid(getInnerX(), getInnerY(), std::vector<float> { getInnerHeight() }, std::vector<float> { getInnerHeight(), getInnerWidth() - getInnerHeight() }),
	rightGrid(getInnerX() + getInnerHeight(), getInnerY(), std::vector<float> { getInnerHeight() * (5.0f / 18), getInnerHeight() * (3.0f / 18), getInnerHeight() * (8.0f / 18), getInnerHeight() * (2.0f / 18) }, std::vector<float> { getInnerWidth() - getInnerHeight() }),
	bottomGrid(getInnerX(), getInnerY(), std::vector<float> { getInnerHeight() * (2.0f / 18) }, std::vector<float> { getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), getInnerHeight() * (2.0f / 18), (getInnerWidth() - getInnerHeight()) - ((getInnerHeight() * (2.0f / 18)) * 7), getInnerHeight() * (2.0f / 18), 2 * (getInnerHeight() * (2.0f / 18)) }),
	volButtonGrid(getInnerX(), getInnerY(), std::vector<float> { 100, 100 }, std::vector<float> { 200 })
{
	this->ren = ren;

	std::string mplusPath = PUtils::getcwd() + "\\assets\\mplus-2p-light.ttf";
	std::string gothicPath = PUtils::getcwd() + "\\assets\\GOTHIC.ttf";

	TTF_Font* mplus = TTF_OpenFont(mplusPath.c_str(), 64);
	TTF_Font* mplusSmall = TTF_OpenFont(mplusPath.c_str(), 16);
	TTF_Font* gothic = TTF_OpenFont(gothicPath.c_str(), 128);

	TTF_SetFontHinting(mplus, TTF_HINTING_LIGHT);
	TTF_SetFontHinting(gothic, TTF_HINTING_NORMAL);

	// AudioPlayer buttons
	std::string playPath = PUtils::getcwd() + "\\assets\\textures\\ap-play.png";
	SDL_Texture* playTexture = IMG_LoadTexture(ren, playPath.c_str());
	ImageButton* play = new ImageButton(playTexture, 0, 0, 64, 64);
	play->setBaseTint(160);
	play->setTintDelta(7);
	addSubscriber(play);

	std::string pausePath = PUtils::getcwd() + "\\assets\\textures\\ap-pause.png";
	SDL_Texture* pauseTexture = IMG_LoadTexture(ren, pausePath.c_str());
	ImageButton* pause = new ImageButton(pauseTexture, 0, 0, 64, 64);
	pause->setBaseTint(160);
	pause->setTintDelta(7);
	addSubscriber(pause);

	std::string stopPath = PUtils::getcwd() + "\\assets\\textures\\ap-stop.png";
	SDL_Texture* stopTexture = IMG_LoadTexture(ren, stopPath.c_str());
	ImageButton* stop = new ImageButton(stopTexture, 0, 0, 64, 64);
	stop->setBaseTint(160);
	stop->setTintDelta(7);
	addSubscriber(stop);

	std::string prevTrackPath = PUtils::getcwd() + "\\assets\\textures\\ap-left.png";
	SDL_Texture* prevTrackTexture = IMG_LoadTexture(ren, prevTrackPath.c_str());
	ImageButton* prevTrack = new ImageButton(prevTrackTexture, 0, 0, 64, 64);
	prevTrack->setBaseTint(160);
	prevTrack->setTintDelta(7);
	addSubscriber(prevTrack);

	std::string nextTrackPath = PUtils::getcwd() + "\\assets\\textures\\ap-right.png";
	SDL_Texture* nextTrackTexture = IMG_LoadTexture(ren, nextTrackPath.c_str());
	ImageButton* nextTrack = new ImageButton(nextTrackTexture, 0, 0, 64, 64);
	nextTrack->setBaseTint(160);
	nextTrack->setTintDelta(7);
	addSubscriber(nextTrack);

	playerControlGrid[0][0].setElement(pause);
	playerControlGrid[0][0].setPadding(27, 33, 6, 0);
	playerControlGrid[0][1].setElement(stop);
	playerControlGrid[0][1].setPadding(6, 33, 27, 0);
	playerControlGrid[1][0].setElement(prevTrack);
	playerControlGrid[1][0].setPadding(27, 0 , 6, 27);
	playerControlGrid[1][1].setElement(nextTrack);
	playerControlGrid[1][1].setPadding(6, 0, 27, 27);
	playerControlGrid.setAlpha(0);
	playerControlGrid.setMinAlpha(0);
	playerControlGrid.setMaxAlpha(255);
	playerControlGrid.setFadeDelta(15);

	// IndexWidget
	IndexWidget* testIndexWidget = new IndexWidget(ren, gothic, 0, 0, 100, 100);
	testIndexWidget->setIndex(96);
	bodyGrid[0][0].setElement(testIndexWidget);
	bodyGrid[0][0].setMinAlpha(0);
	bodyGrid[0][0].setMaxAlpha(255);
	bodyGrid[0][0].setFadeDelta(15);
	

	// Labels/visualizer/etc.
	Label* title = new Label(mplus, 0, 0, 100, 100);
	Label* artist = new Label(mplus, 0, 0, 100, 100);
	title->setClipState(LabelClipState::PAN);
	title->setText("09. TestTestTest - Clean Tears Remix -", ren);
	artist->setClipState(LabelClipState::PAN);
	artist->setText("testArtist", ren);

	// Buttons
	std::string repeatPath = PUtils::getcwd() + "\\assets\\textures\\repeat-64.png";
	SDL_Texture* repeatTexture = IMG_LoadTexture(ren, repeatPath.c_str());
	ImageButton* repeat = new ImageButton(repeatTexture, 0, 0, 64, 64);
	repeat->setBaseTint(160);
	repeat->setTintDelta(7);
	addSubscriber(repeat);

	std::string shufflePath = PUtils::getcwd() + "\\assets\\textures\\shuffle-64.png";
	SDL_Texture* shuffleTexture = IMG_LoadTexture(ren, shufflePath.c_str());
	ImageButton* shuffle = new ImageButton(shuffleTexture, 0, 0, 64, 64);
	shuffle->setBaseTint(160);
	shuffle->setTintDelta(7);
	addSubscriber(shuffle);

	std::string playlistPath = PUtils::getcwd() + "\\assets\\textures\\playlist-64.png";
	SDL_Texture* playlistTexture = IMG_LoadTexture(ren, playlistPath.c_str());
	ImageButton* playlist = new ImageButton(playlistTexture, 0, 0, 64, 64);
	playlist->setBaseTint(160);
	playlist->setTintDelta(7);
	addSubscriber(playlist);

	std::string infoPath = PUtils::getcwd() + "\\assets\\textures\\info-64.png";
	SDL_Texture* infoTexture = IMG_LoadTexture(ren, infoPath.c_str());
	ImageButton* info = new ImageButton(infoTexture, 0, 0, 64, 64);
	info->setBaseTint(160);
	info->setTintDelta(7);
	addSubscriber(info);
	
	// Seek bar
	SeekBar* seekBar = new SeekBar(ren, mplusSmall, 0, 0, 64, 64);
	seekBar->setBaseColor(100, 100, 100);
	seekBar->setTime(0);
	seekBar->setDuration(483);
	addSubscriber(seekBar);

	// Vol buttons
	std::string volUpPath = PUtils::getcwd() + "\\assets\\textures\\menu-up-64.png";
	SDL_Texture* volUpTexture = IMG_LoadTexture(ren, volUpPath.c_str());
	ImageButton* volUp = new ImageButton(volUpTexture, 0, 0, 64, 32);
	volUp->setBaseTint(160);
	volUp->setTintDelta(7);
	addSubscriber(volUp);

	std::string volDownPath = PUtils::getcwd() + "\\assets\\textures\\menu-down-64.png";
	SDL_Texture* volDownTexture = IMG_LoadTexture(ren, volDownPath.c_str());
	ImageButton* volDown = new ImageButton(volDownTexture, 0, 0, 64, 32);
	volDown->setBaseTint(160);
	volDown->setTintDelta(7);
	addSubscriber(volDown);

	// Vol label
	Label* volDb = new Label(mplusSmall, 0, 0, 100, 100);
	volDb->setClipState(LabelClipState::CLIP);
	volDb->setText("0.0dB", ren);

	// Grids
	rightGrid[0][0].setElement(title);
	rightGrid[0][0].setPadding(15, 0, 0, 0);
	rightGrid[1][0].setElement(artist);
	rightGrid[1][0].setPadding(15, 0, 0, 0);
	rightGrid[3][0].setElement(&bottomGrid);
	rightGrid[3][0].setPadding(15, 0, 0, 0);

	bottomGrid[0][0].setElement(playlist);
	bottomGrid[0][0].setPadding(2, 1, 1, 0);
	bottomGrid[0][1].setElement(info);
	bottomGrid[0][1].setPadding(3, 1, 0, 0);
	bottomGrid[0][2].setElement(repeat);
	bottomGrid[0][2].setPadding(2, 2, 2, 2);
	bottomGrid[0][3].setElement(shuffle);
	bottomGrid[0][3].setPadding(1, 2, 2, 1);
	bottomGrid[0][4].setElement(seekBar);
	bottomGrid[0][4].setPadding(4, 0, 0, 0);

	volButtonGrid[0][0].setElement(volUp);
	volButtonGrid[1][0].setElement(volDown);

	bottomGrid[0][5].setElement(&volButtonGrid);
	bottomGrid[0][6].setElement(volDb);

	bodyGrid[0][1].setElement(&rightGrid);

	setBackgroundMinAlpha(0);
	setBackgroundAlpha(0);
	setBackgroundMaxAlpha(150);
	setBackgroundFadeDelta(12);
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
		if (MouseMotionEvent* mouseMotionEvent = dynamic_cast<MouseMotionEvent*>(e))
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

