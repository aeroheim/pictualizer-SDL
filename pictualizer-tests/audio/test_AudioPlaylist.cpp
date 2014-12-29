#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "AudioPlaylist.h"
#include "utils.h"

TEST(AudioPlaylist, getName)
{
	std::wstring name = L"test";
	AudioPlaylist pl(name);
	EXPECT_STREQ(name.c_str(), pl.getName().c_str());
}

TEST(AudioPlaylist, setName)
{
	std::wstring o = L"old";
	std::wstring n = L"new";
	AudioPlaylist pl(o);
	pl.setName(n);
	EXPECT_STREQ(n.c_str(), pl.getName().c_str());
}

TEST(AudioPlaylist, enqueueTrackOnEmptyPlaylist)
{
	std::wstring name = L"test";
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track);

	EXPECT_EQ(1, pl.getSize());
	EXPECT_EQ(track.getDuration(), pl.getDuration());
}

TEST(AudioPlaylist, getTrackValidIndex)
{
	std::wstring name = L"test";
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track);

	EXPECT_TRUE(*pl.getTrack(0) == track);
}

TEST(AudioPlaylist, getTrackInvalidIndex)
{
	std::wstring name = L"test";
	AudioPlaylist pl(name);
	EXPECT_EQ(NULL, pl.getTrack(0));
}

TEST(AudioPlaylist, enqueueTrackOnNonEmptyPlaylist)
{
	std::wstring name = L"test";
	std::wstring path1(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	std::wstring path2(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\tagged.mp3");
	AudioTrack track1(path1);
	AudioTrack track2(path2);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track1);
	pl.enqueueTrack(track2);

	EXPECT_EQ(2, pl.getSize());
	EXPECT_EQ(track1.getDuration() + track2.getDuration(), pl.getDuration());
	EXPECT_TRUE(track1 == *pl.getTrack(0) && track2 == *pl.getTrack(1));
}

TEST(AudioPlaylist, removeTrackInvalidIndex)
{
	std::wstring name = L"test";
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track);
	pl.removeTrack(1);

	EXPECT_EQ(1, pl.getSize());
}

TEST(AudioPlaylist, removeTrackValidIndex)
{
	std::wstring name = L"test";
	std::wstring path(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioTrack track(path);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track);
	pl.removeTrack(0);

	EXPECT_EQ(0, pl.getSize());
	EXPECT_EQ(0, pl.getDuration());
}

TEST(AudioPlaylist, removeTrackPreservesOrder)
{
	std::wstring name = L"test";
	std::wstring path1(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	std::wstring path2(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\tagged.mp3");
	AudioTrack track1(path1);
	AudioTrack track2(path2);
	AudioPlaylist pl(name);

	pl.enqueueTrack(track1);
	pl.enqueueTrack(track2);
	pl.enqueueTrack(track1);
	pl.removeTrack(0);

	EXPECT_TRUE(*pl.getTrack(0) == track2 && *pl.getTrack(1) == track1);
}

TEST(AudioPlaylist, removeAllTracks)
{
	std::wstring name = L"test";
	std::wstring path1(utils::getwstrcwd() + L"\\..\\pictualizer-tests\\audio\\data\\notag.wav");
	AudioPlaylist pl(name);

	for (int i = 0; i < 15; i++)
		pl.enqueueTrack(path1);

	pl.removeAllTracks();

	EXPECT_EQ(0, pl.getSize());
}