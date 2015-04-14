#pragma once

#include "../io/EventObserver.h"
#include "../io/InputListener.h"
#include "../util/PUtils.h"
#include "AudioPlayerStates.h"
#include "AudioPlaylist.h"
#include "AudioTrack.h"
#include <bass.h>
#include <algorithm>
#include <string>
#include <vector>
#include <random>

/*
 *	The AudioPlayer class serves as the main internal audio player for pictualizer. Objects may interact with
 *	and query the AudioPlayer by calling its public methods. Audio related IO must be routed through this class as well.
 */
class AudioPlayer : public EventObserver, public EventSubscriber
{
	public:
		AudioPlayer();
		virtual ~AudioPlayer();

		/* 
		 *	The internal HSTREAM can be retrieved to allow for querying of audio data using BASS. Objects should not
		 *	use BASS' functions to alter the state of the stream, such as stopping or resuming the HSTREAM.
		 */
		HSTREAM* getStream();

		/*
		 *	The main states of AudioPlayer are defined in AudioPlayerStates.h. The state is modified when appropriate
		 *	by the methods within the AudioPlayer class.
		 */
		AudioPlayerState getPlayerState();

		/*	
		 *	AudioPlayer plays tracks based on its current AudioPlaylist. The AudioPlayer class provides methods that allow
		 *	users to navigate through and add/remove AudioPlaylists. Since AudioPlaylist is the main container class for
		 *	AudioTracks, users must first retrieve the current AudioPlaylist from the AudioPlayer in order to add/remove tracks.
		 */
		AudioPlaylist* getCurrentPlaylist();
		AudioPlaylist* getPlaylist(int index);
		int getCurrentPlaylistIndex();
		void addPlaylist(AudioPlaylist playlist);
		void removePlaylist(int index);
		void setCurrentPlaylist(int index);
		void nextPlaylist();
		void prevPlaylist();

		/*
		 *	Among AudioPlayer's basic track navigation functionality, AudioPlayer also allows user to retrieve the current AudioTrack
		 *	instead of having to first retrieve the current AudioPlaylist. Widgets or UI objects may use the playTrack() method to
		 *	select a song to play by index.
		 */
		AudioTrack* getCurrentTrack();
		int getCurrentTrackIndex();
		void playTrack(int index);
		void nextTrack();
		void prevTrack();
		void play();
		void pause();
		void stop();

		/*
		 *	The Repeat states of AudioPlayer are defined in AudioPlayerStates.h. 
		 *	When PLAYLIST repeat is set, the current playlist becomes circular.
		 */
		RepeatState getRepeat();
		void setRepeat(RepeatState s);
	
		/*
	  	 *	The Shuffle states of AudioPlayer are defined in AudioPlayerStates.h.
		 *	AudioPlayer supports shuffling of the main playlist by the following categories: track, folder (TODO), album (TODO).
		 */
		ShuffleState getShuffle();
		void setShuffle(ShuffleState s);

		// Volume
		void setVolume(float vol);
		float getVolume();
	
		// Seeking
		void setPosition(double pos);
		double getPosition();

		double getBASSDuration();

		// BASS callbacks 
		void OnSongEnd();

		void pollStatus();

		void handleEvent(Event* e);

	private:
		HSTREAM stream;

		float volume;

		bool finished;
		
		// Index of the current track in the current playlist.
		int trackIndex;

		// AudioPlayer keeps a list of AudioPlaylists for multiple playlist management and playlist history support.
		std::vector<AudioPlaylist> playlists;
		int playlistIndex;

		// A shuffled list of the current AudioPlaylist is kept to store the shuffled playlist order.
		std::vector<int> shuffledPlaylist;
		int shuffleIndex;

		RepeatState repeatState;
		ShuffleState shuffleState;

		void shuffle();

		void OnTrackRemoved(TrackRemovedEvent* e);
		void OnTrackEnqueued(TrackEnqueuedEvent* e);
		void OnPlaylistCleared(PlaylistClearedEvent* e);
};

