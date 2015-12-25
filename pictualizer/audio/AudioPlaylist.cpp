#pragma once

#include "AudioPlaylist.h"

AudioPlaylist::AudioPlaylist(std::wstring n) : name(n) {}

AudioPlaylist::~AudioPlaylist()
{
}

AudioTrack* AudioPlaylist::getTrack(int index)
{
	if (index >= 0 && index < (int) playlist.size())
		return &playlist[index];

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
	// duration += track.getDuration();
	playlist.push_back(track);

	TrackEnqueuedEvent trackEnqueuedEvent(playlist.size() - 1);
	notify(&trackEnqueuedEvent);
}

void AudioPlaylist::removeTrack(int index)
{
	if (index >= 0 && index < (int) playlist.size())
	{
		duration -= playlist[index].getDuration();
		playlist.erase(playlist.begin() + index);

		TrackRemovedEvent trackRemovedEvent(index);
		notify(&trackRemovedEvent);
	}
}

void AudioPlaylist::removeAllTracks()
{
	duration = 0;
	playlist.clear();

	PlaylistClearedEvent playlistClearedEvent(this);
	notify(&playlistClearedEvent);
}

int AudioPlaylist::getDuration()
{
	return duration;
}

int AudioPlaylist::getSize()
{
	return playlist.size();
}

