#include "AudioTrack.h"

AudioTrack::AudioTrack(std::wstring path) : filePath(path)
{
	/*
	 *	FileRef is too simplistic for our eventual needs, which will involve extracting and
	 *	editing more detailed tags. For now it will suffice, but we should eventually move on to
	 *	using Taglib's format specific subclasses for handling tags. (TODO)
	 */

	TagLib::FileRef track(path.c_str());

	assert(!track.isNull());

	title = track.tag()->title().toCWString();
	artist = track.tag()->artist().toCWString();
	album = track.tag()->album().toCWString();

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

std::wstring AudioTrack::getTitle()
{
	return title;
}

std::wstring AudioTrack::getArtist()
{
	return artist;
}

std::wstring AudioTrack::getAlbum()
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