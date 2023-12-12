#ifndef AUDIO_MANAGER_HEADER
#define AUDIO_MANAGER_HEADER

#include <ArduinoJson.h>

#define NUMBER_OF_SONGS 6

#include "song.h"

class AudioManager
{
private:
    bool playing = false;
    bool muted = false;

    Song song;

    unsigned long init_time = 0;
    unsigned long last_update = 0;
    int current_song = 0;
    unsigned long pause_init_time = 0;

    uint32_t noteToFrequency(int note);
    void restartPlayer();

public:
    AudioManager();

    void init();
    // String handleNoteRequest(StaticJsonDocument<200> doc);
    // String handleSongRequest(StaticJsonDocument<200> doc);
    void update_track(int track_number);

    bool playSong(unsigned int id);
    bool isPlaying() { return playing; }
    void stopSong(bool go_to_next_song);
    void pauseSong();
    void unpauseSong();
    void playTest();
    Song &getSong() { return song; }
    void skipSongs(int number_of_skips);
    unsigned long getCurrentTime()
    {
        if (this->playing)
            return millis() - init_time;
        else
            return (this->pause_init_time - init_time < 0UL) ? 0UL : this->pause_init_time - init_time;
        // return 10;
    }
    void update();
};

#endif