#include "AudioPlaylist.h"

AudioPlaylist::AudioPlaylist(std::wstring n) : name(n) {}

AudioPlaylist::~AudioPlaylist()
{
}

AudioTrack& AudioPlaylist::getTrack(int index)
{
	return *playlist.at(index);
}

std::wstring AudioPlaylist::getName()
{
	return name;
}

void AudioPlaylist::setName(std::wstring n)
{
	name = n;
}

void AudioPlaylist::enqueueTrack(std::wstring file)
{
	AudioTrack track(file);
	duration += track.getDuration();
	playlist.push_back(&track);
}

AudioTrack& AudioPlaylist::removeTrack(int index)
{
	AudioTrack* track = playlist.at(index);
	duration -= track->getDuration();

	playlist.erase(playlist.begin() + index);

	return *track;
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

