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
#include <locale>
#include <codecvt>
#include <filesystem>
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
		std::wstring getPath() const;
		std::wstring getTitle() const;
		std::wstring getArtist() const;
		std::wstring getAlbum() const;
		SDL_Texture* getAlbumArt(SDL_Renderer* ren) const;
		int getDuration() const;

		friend bool operator== (AudioTrack& lhs, AudioTrack& rhs);

	private:
		std::wstring filePath;
		mutable std::wstring title;
		mutable std::wstring artist;
		mutable std::wstring album;
		mutable int duration;

		mutable bool populated;
		void populateMetadata() const;

		SDL_Texture* getID3v2AlbumArt(SDL_Renderer* ren, TagLib::ID3v2::Tag* tag) const;
		SDL_Texture* getAPEAlbumArt(SDL_Renderer* ren, TagLib::APE::Tag* tag) const;
		SDL_Texture* getMP4AlbumArt(SDL_Renderer* ren, TagLib::MP4::Tag* tag) const;
		SDL_Texture* getFLACAlbumArt(SDL_Renderer* ren, TagLib::FLAC::File* file) const;
		SDL_Texture* getASFAlbumArt(SDL_Renderer* ren, TagLib::ASF::Tag* tag) const;
		SDL_Texture* searchDirForAlbumArt(SDL_Renderer* ren) const;

		SDL_Texture* dataToAlbumArt(SDL_Renderer* ren, TagLib::ByteVector& data) const;
};