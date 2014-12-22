#pragma once

#include "AudioTrack.h"
#include <vector>
#include <string>

/*
 *	The AudioPlaylist class serves as a container for AudioTracks. Objects may enqueue or remove
 *	AudioTracks to/from an AudioPlaylist. AudioPlaylist provides methods that can be called to
 *	retrieve the following playlist metadata: total duration, playlist name, and size. (TODO)
 *
 *	Using AudioPlaylists should be the standard for any AudioPlayer or widget implementations that 
 *	store multiple AudioTracks. Pictualizer's default AudioPlayer supports basic playlist and playlist
 *	history functionality by storing a vector of AudioPlaylists. (TODO)
 *
 *	AudioPlaylists must support serialization/deserialization into .m3u playlists. (TODO)
 */
class AudioPlaylist
{
	public:
		AudioPlaylist(std::wstring name);

		AudioTrack& getTrack(int index);

		std::wstring getName();
		void setName(std::wstring name); 

		void enqueueTrack(std::wstring file);
		AudioTrack& removeTrack(int index);
		void removeAllTracks();

		int getDuration();
		int getSize();

	private:
		std::vector<AudioTrack*> playlist;
		std::wstring name;
		int duration;
};