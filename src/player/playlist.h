#ifndef BRAGI_PLAYLIST_H
#define BRAGI_PLAYLIST_H

#include "player/track/track.h"
#include "master.h"

class Playlist final {
public:
	void Add(Track* track);

	void HandleEof();

	u_int16 Skip(const u_int16 num_for_skip);

	void RepeatPlaylist();

	Track* Next(const u_int16 track_index, bool &is_playing);

	bool IsEmpty() const;

	u_int16 Size() const;

	Track* CurrentTrack() const;

	dpp::message Message(const dpp::snowflake &channel_id) const;

private:
	static inline constexpr u_int16 DEFAULT_TRACKS_SIZE = 32;
	u_int16 _max_track_size = DEFAULT_TRACKS_SIZE;
	u_int16 _tracks_size = 0;
	u_int16 _tracks_offset = 0;
	Track** _tracks = new Track*[DEFAULT_TRACKS_SIZE];

	void ResetArray();
};

#endif