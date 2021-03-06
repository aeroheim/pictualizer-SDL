#pragma once

#include "AudioTrack.h"
#include <taglib.h>
#include <mpegfile.h>
#include <apefile.h>
#include <mpcfile.h>
#include <wavpackfile.h>
#include <boost/filesystem.hpp>
#include <assert.h>
#include "../util/PUtils.h"

AudioTrack::AudioTrack(std::string path) : format(AudioTrackFormat::INVALID), filePath(L""), title(L""), artist(L""), album(L""), duration(0), populated(false)
{
	filePath = PUtils::str2wstr(path);

	// Set the track format.
	setTrackFormat();
}

AudioTrack::~AudioTrack() {}

AudioTrackFormat AudioTrack::getFormat() const
{
	return format;
}

std::wstring AudioTrack::getPath() const
{
	return filePath;
}

std::wstring AudioTrack::getTitle() const
{
	if (!populated)
		populateMetadata();

	return title;
}

std::wstring AudioTrack::getArtist() const
{
	if (!populated)
		populateMetadata();

	return artist;
}

std::wstring AudioTrack::getAlbum() const
{
	if (!populated)
		populateMetadata();

	return album;
}

ImageRWops AudioTrack::getAlbumArt(SDL_Renderer* ren) const
{
	TagLib::FileRef track(getPath().c_str());

	ImageRWops albumArt = ImageRWops{ nullptr, nullptr };

	if (TagLib::MPEG::File* mpegFile = dynamic_cast<TagLib::MPEG::File*>(track.file()))
	{
		if (mpegFile->ID3v2Tag())
			albumArt = getID3v2AlbumArt(ren, mpegFile->ID3v2Tag());
		
		if (mpegFile->APETag() && !albumArt.rwops)
			albumArt = getAPEAlbumArt(ren, mpegFile->APETag());
	}
	else if (TagLib::MP4::File* mp4File = dynamic_cast<TagLib::MP4::File*>(track.file()))
	{
		if (mp4File->tag())
			albumArt = getMP4AlbumArt(ren, mp4File->tag());
	}
	else if (TagLib::FLAC::File* flacFile = dynamic_cast<TagLib::FLAC::File*>(track.file()))
	{
		albumArt = getFLACAlbumArt(ren, flacFile);

		if (!albumArt.rwops && flacFile->ID3v2Tag())
			albumArt = getID3v2AlbumArt(ren, flacFile->ID3v2Tag());
	}
	else if (TagLib::ASF::File* asfFile = dynamic_cast<TagLib::ASF::File*>(track.file()))
	{
		albumArt = getASFAlbumArt(ren, asfFile->tag());
	}
	else if (TagLib::APE::File* apeFile = dynamic_cast<TagLib::APE::File*>(track.file()))
	{
		if (apeFile->APETag())
			albumArt = getAPEAlbumArt(ren, apeFile->APETag());
	}
	else if (TagLib::MPC::File* mpcFile = dynamic_cast<TagLib::MPC::File*>(track.file()))
	{
		if (mpcFile->APETag())
			albumArt = getAPEAlbumArt(ren, mpcFile->APETag());
	}
	else if (TagLib::WavPack::File* wavFile = dynamic_cast<TagLib::WavPack::File*>(track.file()))
	{
		if (wavFile->APETag())
			albumArt = getAPEAlbumArt(ren, wavFile->APETag());
	}
	
	if (!albumArt.rwops)
		albumArt = searchDirForAlbumArt(ren);

	return albumArt;
}

int AudioTrack::getDuration() const
{
	if (!populated)
		populateMetadata();

	return duration;
}

bool operator==(AudioTrack& lhs, AudioTrack& rhs)
{
	return lhs.filePath == rhs.filePath;
}

void AudioTrack::setTrackFormat()
{
   std::wstring ext = boost::filesystem::path(filePath).extension().wstring();
	if (ext == L".mp3")
		format = AudioTrackFormat::MP3;
	else if (ext == L".mp2")
		format = AudioTrackFormat::MP2;
	else if (ext == L".mp1")
		format = AudioTrackFormat::MP1;
	else if (ext == L".ogg")
		format = AudioTrackFormat::OGG;
	else if (ext == L".aiff")
		format = AudioTrackFormat::AIFF;
	else if (ext == L".wav")
		format = AudioTrackFormat::WAV;
	else if (ext == L".flac")
		format = AudioTrackFormat::FLAC;
}

void AudioTrack::populateMetadata() const
{
	TagLib::FileRef track(getPath().c_str());

	assert(!populated);
	assert(!track.isNull());

	title = track.tag()->title().toCWString();

	if (title.empty())
	{
		boost::filesystem::path path(filePath);
		title.assign(path.filename().wstring().substr(0, path.filename().size() - path.extension().size()));
	}

	artist = track.tag()->artist().toCWString();

	if (artist.empty())
		artist.assign(L"unknown");

	album = track.tag()->album().toCWString();

	if (album.empty())
		album.assign(L"unknown");

	duration = track.audioProperties()->length();

	populated = true;
}

ImageRWops AudioTrack::getID3v2AlbumArt(SDL_Renderer* ren, TagLib::ID3v2::Tag* tag) const
{
	const TagLib::ID3v2::FrameList& frameList = tag->frameList("APIC");

	if (!frameList.isEmpty())
	{
		// ID3v2 Album art defaults to front cover.
		TagLib::ID3v2::AttachedPictureFrame* frame = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());
		return dataToAlbumArt(ren, frame->picture());
	}

	return ImageRWops{ nullptr, nullptr };
}

ImageRWops AudioTrack::getAPEAlbumArt(SDL_Renderer* ren, TagLib::APE::Tag* tag) const
{
	const TagLib::APE::ItemListMap& itemListMap = tag->itemListMap();

	if (itemListMap.contains("COVER ART (FRONT)"))
	{
		const TagLib::ByteVector nullStringTerminator(1, 0);

		TagLib::ByteVector item = itemListMap["COVER ART (FRONT)"].value();
		int position = item.find(nullStringTerminator);

		if (++position > 0)
			return dataToAlbumArt(ren, item.mid(position));
	}

	return ImageRWops{ nullptr, nullptr };
}

ImageRWops AudioTrack::getMP4AlbumArt(SDL_Renderer* ren, TagLib::MP4::Tag* tag) const
{
	if (tag->itemListMap().contains("covr"))
	{
		TagLib::MP4::CoverArtList coverArtList = tag->itemListMap()["covr"].toCoverArtList();

		if (coverArtList[0].data().size() > 0)
			return dataToAlbumArt(ren, coverArtList[0].data());
	}

	return ImageRWops{ nullptr, nullptr };
}

ImageRWops AudioTrack::getFLACAlbumArt(SDL_Renderer* ren, TagLib::FLAC::File* file) const
{
	const TagLib::List<TagLib::FLAC::Picture*>& pictureList = file->pictureList();

	// Take the first image, which is usually the front cover.
	if (!pictureList.isEmpty())
		return dataToAlbumArt(ren, pictureList[0]->data());

	return ImageRWops{ nullptr, nullptr };
}

ImageRWops AudioTrack::getASFAlbumArt(SDL_Renderer* ren, TagLib::ASF::Tag* tag) const
{
	const TagLib::ASF::AttributeListMap& attributeListMap = tag->attributeListMap();

	if (attributeListMap.contains("WM/Picture"))
	{
		const TagLib::ASF::AttributeList& attributeList = attributeListMap["WM/Picture"];

		if (!attributeList.isEmpty())
		{
			// Take the first image, which is usually the front cover.
			TagLib::ASF::Picture wmpic = attributeList[0].toPicture();

			if (wmpic.isValid())
				return dataToAlbumArt(ren, wmpic.picture());
		}
	}

	return ImageRWops{ nullptr, nullptr };
}

ImageRWops AudioTrack::searchDirForAlbumArt(SDL_Renderer* ren) const
{
	boost::filesystem::path path(filePath);

	if (path.has_parent_path())
	{
		// Get directory file path.
		boost::filesystem::path directoryPath(path.parent_path());

		// Search for "cover" or "folder".
		for (auto it = boost::filesystem::directory_iterator(directoryPath); it != boost::filesystem::directory_iterator(); it++)
		{
			const auto& file = it->path();
			std::wstring filename = file.filename().wstring().substr(0, file.filename().size() - file.extension().size());

			if ((filename == L"cover" || filename == L"folder") && PUtils::pathIsImage(file))
			{
				if (SDL_RWops* albumArt = SDL_RWFromFile(file.string().c_str(), "rb"))
					return ImageRWops{ albumArt, nullptr };
				
				// Stop searching if "cover" or "folder" are invalid images.
				break;
			}
		}

		// Search for any other image in the directory.
		for (auto it = boost::filesystem::directory_iterator(directoryPath); it != boost::filesystem::directory_iterator(); it++)
		{
			const auto& file = it->path();

			if (PUtils::pathIsImage(file))
				if (SDL_RWops* albumArt = SDL_RWFromFile(file.string().c_str(), "rb"))
					return ImageRWops{ albumArt, nullptr };
		}
	}

	return ImageRWops{ nullptr, nullptr };
}


ImageRWops AudioTrack::dataToAlbumArt(SDL_Renderer* ren, TagLib::ByteVector& data) const
{
	uint8_t* buffer = new uint8_t[data.size()];
	memcpy(buffer, data.data(), sizeof(uint8_t) * data.size());

	if (SDL_RWops* albumArt = SDL_RWFromMem(buffer, data.size()))
		return ImageRWops{ albumArt, buffer };
	else
		delete[] buffer;

	return ImageRWops();
}