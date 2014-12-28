#pragma once

#include <fileref.h>
#include <taglib.h>

/*
 *	The AudioTrack class serves as the default object representation of tracks in pictualizer.
 *	AudioTracks contain metadata related to the track, queried using Taglib.
 */
class AudioTrack
{
	public:
		AudioTrack(std::wstring path);
		virtual ~AudioTrack();

		/*
		 *	AudioTrack metadata should be lazy-initialized in order to maintain reasonable 
		 *	performance when loading large playlists. Only when a view requests metadata should
		 *	the respective metadata be retrieved. (TODO)
		 */
		std::wstring getPath();
		std::wstring getTitle();
		std::wstring getArtist();
		std::wstring getAlbum();
		int getDuration();

	private:
		std::wstring filePath;
		std::wstring title;
		std::wstring artist;
		std::wstring album;
		int duration;
};