#include "AudioPlaylist.h"

AudioPlaylist::AudioPlaylist(std::wstring n) : name(n) {}

AudioPlaylist::~AudioPlaylist()
{
}

AudioTrack* AudioPlaylist::getTrack(int index)
{
	if (index >= 0 && (size_t) index < playlist.size())
		return &playlist.at(index);

	return NULL;
}

std::wstring AudioPlaylist::getName()
{
	return name;
}

void AudioPlaylist::setName(std::wstring n)
{
	name = n;
}

void AudioPlaylist::enqueueTrack(AudioTrack track)
{
	duration += track.getDuration();
	playlist.push_back(track);
}

void AudioPlaylist::removeTrack(int index)
{
	if (index >= 0 && (size_t) index < playlist.size())
	{
		duration -= playlist.at(index).getDuration();
		playlist.erase(playlist.begin() + index);;
	}
}

void AudioPlaylist::removeAllTracks()
{
	duration = 0;
	playlist.clear();
}

int AudioPlaylist::getDuration()
{
	return duration;
}

int AudioPlaylist::getSize()
{
	return playlist.size();
}

