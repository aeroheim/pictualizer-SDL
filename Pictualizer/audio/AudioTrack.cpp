#include "AudioTrack.h"

AudioTrack::AudioTrack(std::wstring path) : filePath(path)
{
	TagLib::FileRef track(path.c_str());

	title = track.tag()->title().toCWString();
	artist = track.tag()->artist().toCWString();
	album = track.tag()->artist().toCWString();
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