#include "AudioPlayer.h"

void CALLBACK nextSongCallback(HSYNC handle, DWORD channel, DWORD data, void *user);

AudioPlayer::AudioPlayer()
{
	trackIndex = -1;
	playlistIndex = -1;
	shuffleIndex = -1;

	repeatState = RepeatState::NONE;
	shuffleState = ShuffleState::NONE;
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
	if (playlistIndex >= 0 && (size_t) playlistIndex < playlists.size())
		return &playlists[playlistIndex];

	return NULL;
}

AudioPlaylist* AudioPlayer::getPlaylist(int index)
{
	if (index >= 0 && (size_t)index < playlists.size())
		return &playlists[index];

	return NULL;
}

void AudioPlayer::addPlaylist(AudioPlaylist playlist)
{
	playlists.push_back(playlist);
}

void AudioPlayer::removePlaylist(int index)
{
	playlists.erase(playlists.begin() + index);
}

void AudioPlayer::setCurrentPlaylist(int index)
{
	if (playlists.size() > (size_t) 0 && index >= 0 && (size_t) index < playlists.size())
		playlistIndex = index;
}

void AudioPlayer::nextPlaylist()
{
	if ((size_t)playlistIndex < playlists.size() - 1)
		++playlistIndex;
}

void AudioPlayer::prevPlaylist()
{
	if (playlistIndex > 0)
		--playlistIndex;
}

AudioTrack* AudioPlayer::getCurrentTrack()
{
	AudioPlaylist* currentPlaylist = getCurrentPlaylist();

	if (currentPlaylist)
		return playlists[playlistIndex].getTrack(trackIndex);

	return NULL;
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
		// Setup new HSTREAM.
		BASS_StreamFree(stream);
		stream = BASS_StreamCreateFile(0, playlists[playlistIndex].getTrack(index)->getPath().c_str(), 0, 0, 0);
		BASS_ChannelSetSync(stream, BASS_SYNC_END, NULL, nextSongCallback, this);

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
			if (repeatState == RepeatState::PLAYLIST && (size_t) (shuffleIndex + 1) > shuffledPlaylist.size())
			{
				shuffleIndex = 0;
				trackIndex = shuffledPlaylist[shuffleIndex];
			}
			else if ((size_t)(shuffleIndex + 1) < shuffledPlaylist.size())
				trackIndex = shuffledPlaylist[++shuffleIndex];
		}
		// Non-shuffle standard next track.
		else
		{
			// The playlist is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && trackIndex+ 1 > playlistSize)
				trackIndex = 0;
			else if (trackIndex + 1 < playlistSize)
				++trackIndex;
		}

		playTrack(trackIndex);
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
				trackIndex = shuffledPlaylist[shuffleIndex];
			}
			else if (shuffleIndex != 0)
				trackIndex = shuffledPlaylist[--shuffleIndex];
		}
		// Non-shuffle standard previous track.
		else
		{
			// The playlist is circular if PLAYLIST REPEAT is enabled.
			if (repeatState == RepeatState::PLAYLIST && trackIndex == 0)
				trackIndex = playlistSize - 1;
			else if (trackIndex != 0)
				--trackIndex;
		}

		playTrack(trackIndex);
	}
}

void AudioPlayer::play()
{
	// Playing a stopped song should restart from the beginning.
	bool restart = BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED ? true : false;
	BASS_ChannelPlay(stream, restart);
}

void AudioPlayer::pause()
{
	BASS_ChannelPause(stream);
}

void AudioPlayer::stop()
{
	BASS_ChannelStop(stream);
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
	((AudioPlayer*)user)->OnSongEnd();
}

void AudioPlayer::OnSongEnd()
{
	if (repeatState == RepeatState::SONG)
		play();
	else
		nextTrack();
}