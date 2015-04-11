#include "AudioTrack.h"

AudioTrack::AudioTrack(std::string path)
{
	/*
	 *	FileRef is too simplistic for our eventual needs, which will involve extracting and
	 *	editing more detailed tags. For now it will suffice, but we should eventually move on to
	 *	using Taglib's format specific subclasses for handling tags. (TODO)
	 */

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstrpath = converter.from_bytes(path);

	TagLib::FileRef track(wstrpath.c_str());

	assert(!track.isNull());

	filePath = wstrpath;
	title = track.tag()->title().toCString();
	artist = track.tag()->artist().toCString();
	album = track.tag()->album().toCString();

	// Duration is ignored for now.
	duration = 0;
}

AudioTrack::~AudioTrack()
{
}

std::wstring AudioTrack::getPath()
{
	return filePath;
}

std::string AudioTrack::getTitle()
{
	return title;
}

std::string AudioTrack::getArtist()
{
	return artist;
}

std::string AudioTrack::getAlbum()
{
	return album;
}

int AudioTrack::getDuration()
{
	return duration;
}

bool operator==(AudioTrack& lhs, AudioTrack& rhs)
{
	return lhs.filePath == rhs.filePath;
}