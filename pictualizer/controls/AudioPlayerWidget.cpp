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

	// AudioPlayer buttons
	PTextureButton* playPause= new PTextureButton(PTextureType::AP_PAUSE, 0, 0, 64, 64);
	playPause->setBaseTint(160);
	playPause->setTintDelta(7);
	addSubscriber(playPause);

	// playPause->setTexture(PTextureType::AP_PLAY);

	PTextureButton* stop = new PTextureButton(PTextureType::AP_STOP, 0, 0, 64, 64);
	stop->setBaseTint(160);
	stop->setTintDelta(7);
	addSubscriber(stop);

	PTextureButton* prevTrack = new PTextureButton(PTextureType::AP_LEFT, 0, 0, 64, 64);
	prevTrack->setBaseTint(160);
	prevTrack->setTintDelta(7);
	addSubscriber(prevTrack);

	PTextureButton* nextTrack = new PTextureButton(PTextureType::AP_RIGHT, 0, 0, 64, 64);
	nextTrack->setBaseTint(160);
	nextTrack->setTintDelta(7);
	addSubscriber(nextTrack);

	playerControlGrid[0][0].setElement(playPause);
	playerControlGrid[0][0].setPadding(0.3f, 0.3f, 0, 0);
	playerControlGrid[0][1].setElement(stop);
	playerControlGrid[0][1].setPadding(0, 0.3f, 0.3f, 0);
	playerControlGrid[1][0].setElement(prevTrack);
	playerControlGrid[1][0].setPadding(0.3f, 0, 0.05f, 0.3f);
	playerControlGrid[1][1].setElement(nextTrack);
	playerControlGrid[1][1].setPadding(0.05f, 0, 0.3f, 0.3f);
	playerControlGrid.setAlpha(0);
	playerControlGrid.setMinAlpha(0);
	playerControlGrid.setMaxAlpha(255);
	playerControlGrid.setFadeDelta(15);

	// IndexWidget
	IndexWidget* testIndexWidget = new IndexWidget(ren, PFontType::CENTURYGOTHIC, 0, 0, 100, 100);
	testIndexWidget->setIndex(47);
	bodyGrid[0][0].setElement(testIndexWidget);
	bodyGrid[0][0].setMinAlpha(0);
	bodyGrid[0][0].setMaxAlpha(255);
	bodyGrid[0][0].setFadeDelta(15);
	

	// Labels/visualizer/etc.
	Label* title = new Label(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100);
	Label* artist = new Label(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100);
	title->setClipState(LabelClipState::PAN);
	title->setText("09. TestTestTest - Clean Tears Remix -");
	artist->setClipState(LabelClipState::PAN);
	artist->setText("testArtist");

	// Buttons	
	PTextureButton* repeat = new PTextureButton(PTextureType::AP_REPEAT, 0, 0, 64, 64);
	repeat->setButtonStyle(ButtonStyle::TOGGLE);
	repeat->setBaseTint(160);
	repeat->setTintDelta(7);
	addSubscriber(repeat);

	PTextureButton* shuffle = new PTextureButton(PTextureType::AP_SHUFFLE, 0, 0, 64, 64);
	shuffle->setButtonStyle(ButtonStyle::TOGGLE);
	shuffle->setBaseTint(160);
	shuffle->setTintDelta(7);
	addSubscriber(shuffle);

	PTextureButton* playlist = new PTextureButton(PTextureType::AP_PLAYLIST, 0, 0, 64, 64);
	playlist->setBaseTint(160);
	playlist->setTintDelta(7);
	addSubscriber(playlist);

	PTextureButton* info = new PTextureButton(PTextureType::AP_INFO, 0, 0, 64, 64);
	info->setBaseTint(160);
	info->setTintDelta(7);
	addSubscriber(info);
	
	// Seek bar
	SeekBar* seekBar = new SeekBar(ren, PFontType::MPLUSLIGHT, 0, 0, 64, 64);
	seekBar->setBaseColor(100, 100, 100);
	seekBar->setTime(0);
	seekBar->setDuration(483);
	addSubscriber(seekBar);

	// Vol buttons
	PTextureButton* volUp = new PTextureButton(PTextureType::AP_VOL_UP, 0, 0, 64, 32);
	volUp->setBaseTint(160);
	volUp->setTintDelta(7);
	addSubscriber(volUp);

	PTextureButton* volDown = new PTextureButton(PTextureType::AP_VOL_DOWN, 0, 0, 64, 32);
	volDown->setBaseTint(160);
	volDown->setTintDelta(7);
	addSubscriber(volDown);

	// Vol label
	Label* volDb = new Label(ren, PFontType::MPLUSLIGHT, 0, 0, 100, 100);
	volDb->setClipState(LabelClipState::CLIP);
	volDb->setText("0.0dB");

	// Grids
	rightGrid[0][0].setElement(title);
	rightGrid[1][0].setElement(artist);
	rightGrid[3][0].setElement(&bottomGrid);

	bottomGrid[0][0].setElement(playlist);
	bottomGrid[0][1].setElement(info);
	bottomGrid[0][2].setElement(repeat);
	bottomGrid[0][2].setPadding(0.15f, 0.15f, 0.15f, 0.15f);
	bottomGrid[0][3].setElement(shuffle);
	bottomGrid[0][3].setPadding(0.1f, 0.1f, 0.1f, 0.1f);
	bottomGrid[0][4].setElement(seekBar);
	bottomGrid[0][4].setPadding(0.02f, 0, 0, 0);

	volButtonGrid[0][0].setElement(volUp);
	volButtonGrid[0][0].setPadding(0, 0.2f, 0, 0);
	volButtonGrid[1][0].setElement(volDown);
	volButtonGrid[1][0].setPadding(0, 0, 0, 0.2f);

	bottomGrid[0][5].setElement(&volButtonGrid);
	bottomGrid[0][5].setPadding(0, 0.08f, 0, 0);
	bottomGrid[0][6].setElement(volDb);
	bottomGrid[0][6].setPadding(0, 0.08f, 0, 0);

	bodyGrid[0][1].setElement(&rightGrid);
	bodyGrid[0][1].setPadding(0.03f, 0, 0, 0);

	setBackgroundMinAlpha(150);
	setBackgroundAlpha(0);
	setBackgroundMaxAlpha(200);
	setBackgroundFadeDelta(4.5);
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

