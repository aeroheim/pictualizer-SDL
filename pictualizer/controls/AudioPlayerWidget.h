#pragma once

#include <iomanip>
#include <sstream>
#include <filesystem>
#include "PWidget.h"
#include "IndexWidget.h"
#include "PTextureButton.h"
#include "ImageButton.h"
#include "SeekBar.h"
#include "WaveformVisualizer.h"
#include "SpectrumVisualizer.h"
#include "AudioPlayerWidgetStates.h"
#include "PControlContainer.h"
#include "../audio/AudioPlayer.h"
#include "../assets/fonts/PFonts.h"
#include "../assets/textures/PTextures.h"
#include "../util/PUtils.h"

class AudioPlayerWidget : public PWidget
{
	public:
		AudioPlayerWidget(SDL_Renderer* ren, AudioPlayer* audioPlayer, float x, float y, float w, float h);
		~AudioPlayerWidget();

		void setX(float x);
		void setY(float y);
		void setWidth(float w);
		void setHeight(float h);

		void draw(SDL_Renderer* ren = nullptr);

		void handleEvent(Event* e);

	private:
		AudioPlayerWidgetDisplayState displayState;
		AudioPlayerWidgetVisualizationState visualizationState;
		AudioPlayer* audioPlayer;
		SDL_Renderer* ren;

		GridPanel playerControlGrid;
		GridPanel bodyGrid;
		GridPanel rightGrid;
		GridPanel bottomGrid;
		GridPanel volButtonGrid;

		PControlContainer leftControlsContainer;
		PControlContainer visualizationContainer;

		ImageTexture defaultAlbumArt;
		std::wstring album;

		IndexWidget indexWidget;
		Image albumArt;
		SeekBar seekBar;
		Label title;
		Label artist;
		Label volDb;
		PTextureButton playPause;
		PTextureButton stop;
		PTextureButton prevTrack;
		PTextureButton nextTrack;
		PTextureButton repeat;
		PTextureButton shuffle;
		PTextureButton playlist;
		PTextureButton info;
		PTextureButton volUp;
		PTextureButton volDown;

		WaveformVisualizer waveformVisualizer;
		SpectrumVisualizer spectrumVisualizer;

		const float ALBUM_ART_MIN_ALPHA = 100.0f;
		const float VOL_DELTA = 0.05f;
		int frameCount;
		bool seeking;

		void enqueueTrack(const std::string& path);

		SDL_Texture* getDefaultAlbumArt();

		void OnMouseUp(MouseUpEvent* e);
		void OnMouseMotion(MouseMotionEvent* e);
		void OnWindowResized(WindowResizedEvent* e);
		void OnFileDrop(FileDropEvent* e);
		void OnDirectoryDrop(FileDropEvent* e);
		void OnButtonPressed(ButtonPressedEvent* e);
		void OnButtonToggled(ButtonToggledEvent* e);
		void OnSeek(SeekEvent* e);
		void OnSeekRequest(SeekRequestEvent* e);
		void OnNewTrack(NewTrackEvent* e);
		void OnPlayerStarted(PlayerStartedEvent* e);
		void OnPlayerPaused(PlayerPausedEvent* e);
		void OnPlayerStopped(PlayerStoppedEvent* e);
		void OnVolumeChanged(VolumeChangedEvent* e);
};