#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <taglib.h>
#include <fileref.h>
#include <mpegfile.h>
#include <flacfile.h>
#include <mp4file.h>
#include <asffile.h>
#include <apefile.h>
#include <mpcfile.h>
#include <wavpackfile.h>
#include <id3v2tag.h>
#include <apetag.h>
#include <assert.h>
#include <string>
#include <filesystem>
#include "AudioTrackStates.h"
#include "../controls/Image.h"
#include "../io/EventObserver.h"
#include "../util/PUtils.h"

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
		 *	AudioTrack metadata are lazy-initialized in order to maintain reasonable 
		 *	performance when loading large playlists. The metadata for an audioTrack is
		 *	populated only when a getter has been called.
		 */
		AudioTrackFormat getFormat() const;
		std::wstring getPath() const;
		std::wstring getTitle() const;
		std::wstring getArtist() const;
		std::wstring getAlbum() const;
		ImageRWops getAlbumArt(SDL_Renderer* ren) const;
		int getDuration() const;

		friend bool operator== (AudioTrack& lhs, AudioTrack& rhs);

	private:
		AudioTrackFormat format;
		std::wstring filePath;
		mutable std::wstring title;
		mutable std::wstring artist;
		mutable std::wstring album;
		mutable int duration;

		void setTrackFormat();

		mutable bool populated;
		void populateMetadata() const;

		ImageRWops getID3v2AlbumArt(SDL_Renderer* ren, TagLib::ID3v2::Tag* tag) const;
		ImageRWops getAPEAlbumArt(SDL_Renderer* ren, TagLib::APE::Tag* tag) const;
		ImageRWops getMP4AlbumArt(SDL_Renderer* ren, TagLib::MP4::Tag* tag) const;
		ImageRWops getFLACAlbumArt(SDL_Renderer* ren, TagLib::FLAC::File* file) const;
		ImageRWops getASFAlbumArt(SDL_Renderer* ren, TagLib::ASF::Tag* tag) const;
		ImageRWops searchDirForAlbumArt(SDL_Renderer* ren) const;

		ImageRWops dataToAlbumArt(SDL_Renderer* ren, TagLib::ByteVector& data) const;
};