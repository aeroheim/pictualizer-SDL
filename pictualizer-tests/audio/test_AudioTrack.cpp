#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "AudioTrack.h"
#include "utils.h"

TEST(AudioTrack, Initialization)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
}

TEST(AudioTrack, getPath)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	ASSERT_STREQ(path.c_str(), track.getPath().c_str());
}

TEST(AudioTrack, getTitle)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\tagged.mp3");
	AudioTrack track(path);
	EXPECT_STREQ(L"Groove Loop", track.getTitle().c_str());
}

TEST(AudioTrack, getArtist)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\tagged.mp3");
	AudioTrack track(path);
	EXPECT_STREQ(L"Novelty Christmas Group", track.getArtist().c_str());
}

TEST(AudioTrack, getAlbum)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\tagged.mp3");
	AudioTrack track(path);
	EXPECT_STREQ(L"70s Christmas Tunes", track.getAlbum().c_str());
}

TEST(AudioTrack, getDuration)
{
	EXPECT_EQ(true, false);
}

TEST(AudioTrack, getEmptyTitle)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	EXPECT_STREQ(track.getTitle().c_str(), L"");
}

TEST(AudioTrack, getEmptyArtist)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	EXPECT_STREQ(track.getArtist().c_str(), L"");
}

TEST(AudioTrack, getEmptyAlbum)
{
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	EXPECT_STREQ(track.getAlbum().c_str(), L"");
}
