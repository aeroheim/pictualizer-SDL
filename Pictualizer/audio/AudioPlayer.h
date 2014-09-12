#include <bass.h>
#include <string>
#include <vector>

enum class State {PLAYING, PAUSED, STOPPED};

class AudioPlayer
{
public:
	HCHANNEL* getBASSChannel();
	void next();
	void previous();
	void play();
	void pause();
	void stop();
	void repeat(bool on);
	void shuffle(bool on);
	void playSong(std::wstring file);
	void playSong(int index);
	void enqueueSong(std::wstring file);
	std::wstring removeSong(int index);

	void setVolume(float v);
	float getVolume();
	
	void setPosition(float p);
	float getPosition();

	std::wstring getTitle(int index);
	std::wstring getArtist(int index);
	std::wstring getAlbum(int index);
	int getTrackNumber(int index);
	
	// Current song
	int getPlaylistPosition();

	State getState();

	bool repeat = false;
	bool shuffle = false;
private:
	std::vector<std::wstring> playlist;
	std::vector<int> shuffleList;

	HCHANNEL channel;
	DWORD tagFormat;
};
