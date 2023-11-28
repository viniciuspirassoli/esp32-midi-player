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

    unsigned long init_time;
    unsigned long last_update = 0;

    uint32_t noteToFrequency(int note);
    void restartPlayer();

public:
    AudioManager();

    void init();
    String handleNoteRequest(StaticJsonDocument<200> doc);
    String handleSongRequest(StaticJsonDocument<200> doc);

    bool playSong(int id);
    void stopSong();
    void playTest();

    void update();
};

#endif