#include "AudioPlayer.h"

void CALLBACK nextSongCallback(HSYNC handle, DWORD channel, DWORD data, void *user);

AudioPlayer::AudioPlayer()
{
	trackIndex = -1;
	playlistIndex = -1;
	shuffleIndex = -1;

	// Use default playlist on startup.
	addPlaylist(AudioPlaylist(L"default playlist"));
	nextPlaylist();

	repeatState = RepeatState::NONE;
	shuffleState = ShuffleState::NONE;

	BASS_SetVolume(0.1f);
}

AudioPlayer::~AudioPlayer()
{
	BASS_StreamFree(stream);
}

HCHANNEL* AudioPlayer::getStream()
{
	return &stream;
}

AudioPlayerState AudioPlayer::getPlayerState()
{
	switch (BASS_ChannelIsActive(stream))
	{
		case BASS_ACTIVE_PLAYING:
			return AudioPlayerState::PLAYING;
		case BASS_ACTIVE_PAUSED:
			return AudioPlayerState::PAUSED;
		case BASS_ACTIVE_STOPPED:
			return AudioPlayerState::STOPPED;
		default:
			return AudioPlayerState::STOPPED;
	}
}

AudioPlaylist* AudioPlayer::getCurrentPlaylist()
{
	if (playlistIndex >= 0 && playlistIndex < (int) playlists.size())
		return &playlists[playlistIndex];

	return NULL;
}

AudioPlaylist* AudioPlayer::getPlaylist(int index)
{
	if (index >= 0 && index < (int) playlists.size())
		return &playlists[index];

	return NULL;
}

void AudioPlayer::addPlaylist(AudioPlaylist playlist)
{
	playlists.push_back(playlist);
}

void AudioPlayer::removePlaylist(int index)
{
	// TODO: figure out which playlist to set ourselves to if current one is removed.
	if (playlistIndex >= index)
	{
		if (playlistIndex == index)
			unsubscribeFrom(getCurrentPlaylist());

		--playlistIndex;
	}

	playlists.erase(playlists.begin() + index);
}

void AudioPlayer::setCurrentPlaylist(int index)
{
	if ((int) playlists.size() > 0 && index >= 0 && index < (int) playlists.size())
	{
		// Set current playlistIndex to new index.
		playlistIndex = index;

		// Unsubscribe from the old playlist if possible.
		AudioPlaylist* currentPlaylist = getCurrentPlaylist();

		if (currentPlaylist)
			unsubscribeFrom(currentPlaylist);

		// Subscribe to new playlist and set appropriate trackIndex based on its size.
		AudioPlaylist* newPlaylist = getPlaylist(playlistIndex = index);
		subscribeTo(newPlaylist);

		trackIndex == newPlaylist->getSize() == 0 ? -1 : 0;

		// Shuffle again for new playlist if necessary.
		shuffledPlaylist.clear();
		shuffledPlaylist.resize(currentPlaylist->getSize());

		if (shuffleState == ShuffleState::PLAYLIST)
			shuffle();
	}
}

void AudioPlayer::nextPlaylist()
{
	if (playlistIndex < (int) playlists.size() - 1)
		setCurrentPlaylist(playlistIndex + 1);
}

void AudioPlayer::prevPlaylist()
{
	if (playlistIndex > 0)
		setCurrentPlaylist(playlistIndex - 1);
}

AudioTrack* AudioPlayer::getCurrentTrack()
{
	AudioPlaylist* currentPlaylist = getCurrentPlaylist();

	if (currentPlaylist)
		return playlists[playlistIndex].getTrack(trackIndex);

	return nullptr;
}

int AudioPlayer::getCurrentTrackIndex()
{
	return trackIndex;
}

void AudioPlayer::playTrack(int index)
{
	AudioPlaylist* currentPlaylist = getCurrentPlaylist();
	
	if (currentPlaylist && index >= 0 && index < currentPlaylist->getSize())
	{
		// Set current trackIndex to new index.
		trackIndex = index;

		// Setup new HSTREAM.
		BASS_StreamFree(stream);
		stream = BASS_StreamCreateFile(0, getCurrentTrack()->getPath().c_str(), 0, 0, 0);
		BASS_ChannelSetSync(stream, BASS_SYNC_END, NULL, nextSongCallback, this);

		NewTrackEvent newTrackEvent(getCurrentTrack());
		notify(&newTrackEvent);

		// Play the new song.
		play();
	}
}

void AudioPlayer::nextTrack()
{
	int playlistSize = playlists[playlistIndex].getSize();

	if (playlistSize > 1)
	{
		// Use the shuffle list to select the next track index if shuffling.
		if (shuffleState != ShuffleState::NONE)
		{
			// The shuffle list is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && shuffleIndex + 1 > (int) shuffledPlaylist.size())
			{
				shuffleIndex = 0;
				playTrack(shuffledPlaylist[shuffleIndex]);
			}
			else if (shuffleIndex + 1 < (int) shuffledPlaylist.size())
				playTrack(shuffledPlaylist[++shuffleIndex]);
		}
		// Non-shuffle standard next track.
		else
		{
			// The playlist is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && trackIndex + 1 > playlistSize)
				playTrack(0);
			else if (trackIndex + 1 < playlistSize)
				playTrack(trackIndex + 1);
		}
	}
}

void AudioPlayer::prevTrack()
{
	int playlistSize = playlists[playlistIndex].getSize();

	if (playlistSize > 1)
	{
		// Use the shuffle list to select the previous track if shuffling.
		if (shuffleState != ShuffleState::NONE)
		{
			// The shuffle list is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && shuffleIndex == 0)
			{
				shuffleIndex = shuffledPlaylist.size() - 1;
				playTrack(shuffledPlaylist[shuffleIndex]);
			}
			else if (shuffleIndex != 0)
				playTrack(shuffledPlaylist[--shuffleIndex]);
		}
		// Non-shuffle standard previous track.
		else
		{
			// The playlist is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && trackIndex == 0)
				playTrack(playlistSize - 1);
			else if (trackIndex != 0)
				playTrack(trackIndex - 1);
		}
	}
}

void AudioPlayer::play()
{
	// Playing a stopped song should restart from the beginning.
	bool restart = BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED ? true : false;
	BASS_ChannelPlay(stream, restart);

	PlayerStartedEvent playerStartedEvent;
	notify(&playerStartedEvent);
}

void AudioPlayer::pause()
{
	BASS_ChannelPause(stream);

	PlayerPausedEvent playerPausedEvent;
	notify(&playerPausedEvent);
}

void AudioPlayer::stop()
{
	BASS_ChannelStop(stream);

	PlayerStoppedEvent playerStoppedEvent;
	notify(&playerStoppedEvent);
}

RepeatState AudioPlayer::getRepeat()
{
	return repeatState;
}

void AudioPlayer::setRepeat(RepeatState s)
{
	repeatState = s;
}

ShuffleState AudioPlayer::getShuffle()
{
	return shuffleState;
}

void AudioPlayer::setShuffle(ShuffleState s)
{
	shuffleState = s;

	if (s == ShuffleState::PLAYLIST)
		shuffle();
}

/*
 *	BASS' volume ranges from 0(silent) to 1(max).
 */
void AudioPlayer::setVolume(float v)
{
	BASS_SetVolume(v);
}

float AudioPlayer::getVolume()
{
	return BASS_GetVolume();
}

/*
 *	The position given should be specified in seconds with decimals.
 */
void AudioPlayer::setPosition(double p)
{
	BASS_ChannelSetPosition(stream, BASS_ChannelSeconds2Bytes(stream, p), BASS_POS_BYTE);
}

/*
 *	The position is returned in seconds with decimals.
 */
double AudioPlayer::getPosition()
{
	return BASS_ChannelBytes2Seconds(stream, BASS_ChannelGetPosition(stream, BASS_POS_BYTE));
}

/*
 *	Performs a Fisher-Yates shuffle on the shuffle list.
 */
void AudioPlayer::shuffle()
{
	std::default_random_engine rng;

	int playlistSize = playlists[playlistIndex].getSize();

	for (int i = 0; i < playlistSize; i++)
		shuffledPlaylist[i] = i;

	for (int i = playlistSize - 1; i >= 0; i--)
	{
		std::uniform_int_distribution<int> distribution(0, i);
		int j = distribution(rng);
		std::swap(shuffledPlaylist[j], shuffledPlaylist[i]);
	}

	shuffleIndex = 0;
}

/*
 *	Callback invoked by BASS when a song finishes.
 */
void CALLBACK nextSongCallback(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	((AudioPlayer*) user)->OnSongEnd();
}

void AudioPlayer::OnSongEnd()
{
	PlayerStoppedEvent playerStoppedEvent;
	notify(&playerStoppedEvent);

	if (repeatState == RepeatState::SONG)
		play();
	else
		nextTrack();
}

void AudioPlayer::handleEvent(Event* e)
{
	if (TrackRemovedEvent* trackRemovedEvent = dynamic_cast<TrackRemovedEvent*>(e))
	{
		OnTrackRemoved(trackRemovedEvent);
		e->handled = true;
	}
	else if (TrackEnqueuedEvent* trackEnqueuedEvent = dynamic_cast<TrackEnqueuedEvent*>(e))
	{
		OnTrackEnqueued(trackEnqueuedEvent);
		e->handled = true;
	}
	else if (PlaylistClearedEvent* playlistClearedEvent = dynamic_cast<PlaylistClearedEvent*>(e))
	{
		OnPlaylistCleared(playlistClearedEvent);
		e->handled = true;
	}
}

void AudioPlayer::OnTrackRemoved(TrackRemovedEvent* e)
{
	if (trackIndex >= e->index)
		--trackIndex;

	shuffledPlaylist.pop_back();

	if (shuffleState == ShuffleState::PLAYLIST)
		shuffle();
}

void AudioPlayer::OnTrackEnqueued(TrackEnqueuedEvent* e)
{
	if (trackIndex >= e->index)
		++trackIndex;
	else if (trackIndex == -1)
	{
		++trackIndex;
		playTrack(trackIndex);
	}

	shuffledPlaylist.push_back(0);

	if (shuffleState == ShuffleState::PLAYLIST)
		shuffle();
}

void AudioPlayer::OnPlaylistCleared(PlaylistClearedEvent* e)
{
	trackIndex = -1;
}
