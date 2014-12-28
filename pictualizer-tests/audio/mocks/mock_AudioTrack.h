#include "gmock/gmock.h"
#include "AudioTrack.h"

class MockAudioTrack : AudioTrack
{
	public:
		MOCK_METHOD0(getPath, std::wstring());
		MOCK_METHOD0(getTitle, std::wstring());
		MOCK_METHOD0(getArtist, std::wstring());
		MOCK_METHOD0(getAlbum, std::wstring());
		MOCK_METHOD0(getDuration, int());
};