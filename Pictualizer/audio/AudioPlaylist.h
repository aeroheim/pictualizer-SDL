#pragma once

#include "AudioTrack.h"
#include <vector>

using std::vector;
using std::wstring;

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
		AudioPlaylist();
		~AudioPlaylist();

		AudioTrack& getTrack(int index);

		wstring getName();
		void setName();

		void enqueueTrack(wstring file);
		wstring removeTrack(int index);

		int getDuration();
		int getSize();

	private:
		vector<AudioTrack*> playlist;
		wstring name;
		int duration;
};