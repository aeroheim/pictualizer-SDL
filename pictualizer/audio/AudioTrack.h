#pragma once

#include "../io/EventObserver.h"
#include <fileref.h>
#include <taglib.h>
#include <assert.h>
#include <string>
#include <locale>
#include <codecvt>

/*
 *	The AudioTrack class serves as the default object representation of tracks in pictualizer.
 *	AudioTracks contain metadata related to the track, queried using Taglib.
 */
class AudioTrack : public EventObserver
{
	public:
		AudioTrack(std::string path);
		virtual ~AudioTrack();

		/*
		 *	AudioTrack metadata should be lazy-initialized in order to maintain reasonable 
		 *	performance when loading large playlists. Only when a view requests metadata should
		 *	the respective metadata be retrieved. (TODO)
		 */
		std::wstring getPath();
		std::string getTitle();
		std::string getArtist();
		std::string getAlbum();
		int getDuration();
		friend bool operator== (AudioTrack& lhs, AudioTrack& rhs);

	private:
		std::wstring filePath;
		std::string title;
		std::string artist;
		std::string album;
		int duration;
};