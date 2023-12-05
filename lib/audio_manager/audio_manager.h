#ifndef AUDIO_MANAGER_HEADER
#define AUDIO_MANAGER_HEADER

#include <ArduinoJson.h>

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

    uint32_t noteToFrequency(int note);
    void restartPlayer();

public:
    AudioManager();

    void init();
    String handleNoteRequest(StaticJsonDocument<200> doc);
    String handleSongRequest(StaticJsonDocument<200> doc);

    bool playSong(unsigned int id);
    bool isPlaying() { return playing; }
    void stopSong(bool go_to_next_song);
    void pauseSong();
    void playTest();
    Song &getSong() { return song; }
    void skipSongs(int number_of_skips);
    unsigned long getCurrentTime()
    {
        return (this->last_update - this->init_time);
        // return 10;
    }

    void update();
};

#endif