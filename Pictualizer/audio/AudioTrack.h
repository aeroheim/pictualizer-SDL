#pragma once

#include <Taglib.h>

using std::wstring;

/*
 *	The AudioTrack class serves as the default object representation of tracks in pictualizer.
 *	AudioTracks contain metadata related to the track, queried using Taglib.
 */
class AudioTrack
{
	public:
		AudioTrack(wstring filePath);

		/*
		 *	AudioTrack metadata should be lazy-initialized in order to maintain reasonable 
		 *	performance when loading large playlists. Only when a view requests metadata should
		 *	the respective metadata be retrieved.
		 */
		wstring getPath();
		wstring getTitle();
		wstring getArtist();
		wstring getAlbum();

	private:
		wstring path;
		wstring title;
		wstring artist;
		wstring album;
};