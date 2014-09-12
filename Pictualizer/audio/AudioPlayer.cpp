#include "AudioPlayer.h"

HCHANNEL* AudioPlayer::getBASSChannel()
{
	throw std::runtime_error("Not implemented");
	return NULL;
}

void AudioPlayer::next()
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::previous()
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::play()
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::pause()
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::stop()
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::setRepeat(bool on)
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::setShuffle(bool on)
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::playSong(std::wstring file)
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::playSong(int index)
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::enqueueSong(std::wstring file)
{
	throw std::runtime_error("Not implemented");
}

std::wstring AudioPlayer::removeSong(int index)
{
	throw std::runtime_error("Not implemented");
}

void AudioPlayer::setVolume(float v)
{
	throw std::runtime_error("Not implemented");
}

float AudioPlayer::getVolume()
{
	throw std::runtime_error("Not implemented");
	return 0;
}

void AudioPlayer::setPosition(float p)
{
	throw std::runtime_error("Not implemented");
}

float AudioPlayer::getPosition()
{
	throw std::runtime_error("Not implemented");
}

std::wstring AudioPlayer::getTitle(int index)
{
	throw std::runtime_error("Not implemented");
}

std::wstring AudioPlayer::getArtist(int index)
{
	throw std::runtime_error("Not implemented");
}

std::wstring AudioPlayer::getAlbum(int index)
{
	throw std::runtime_error("Not implemented");
}

int AudioPlayer::getTrackNumber(int index)
{
	throw std::runtime_error("Not implemented");
}

int AudioPlayer::getPlaylistPosition()
{
	throw std::runtime_error("Not implemented");
}

State AudioPlayer::getState()
{
	throw std::runtime_error("Not implemented");
}
