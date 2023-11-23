#include "audio_manager.h"

#include <Arduino.h>
// #include <JsonTranslator.h>
#include <LittleFS.h>
#include <string>

#include "pins.h"

int last_event[CHANNELS];
int buzzers[] = {OUTPUT_1, OUTPUT_2, OUTPUT_3, OUTPUT_4};

double AudioManager::noteToFrequency(int note)
{
    return 440 * exp2((note - 69) / 12.0);
}

AudioManager::AudioManager() {}

void AudioManager::playSong(int id)
{
    String filename;
    switch (id)
    {
    case 0:
        filename = "zelda_secret.miniMid";
        break;
    case 1:
        filename = "sonic_starlight_zone.miniMid";
        break;
    case 2:
        filename = "smb3_overworld.miniMid";
        break;
    case 3:
        filename = "smb_overworld.miniMid";
        break;
    case 4:
        filename = "mii_channel_theme.miniMid";
        break;
    case 5:
        filename = "zelda_overworld.miniMid";
        break;
    case 6:
        filename = "careless_whisper.miniMid";
        break;
    case 7:
        filename = "tetris_theme_a.miniMid";
        break;
    default:
        playing = false;
        return;
    }

    this->song.load_from_file(filename);

    playing = true;
    for (int i = 0; i < CHANNELS; i++)
        last_event[i] = 0;
    init_time = millis();

    this->update();
}

void AudioManager::playTest()
{
    Song smb_song = Song(4);

    smb_song.active_channels = 4;

    int number_of_notes = 3;
    int notes[3] = {142, 284, 213};
    unsigned int durations[3] = {100, 100, 100};
    unsigned int timestamps[3] = {0, 150, 300};

    for (size_t i = 0; i < 4; i++)
    {
        Track &loop_track = smb_song.get_track(i);
        loop_track.load_from_values(notes, timestamps, durations, 3);
    }

    this->song = smb_song;

    playing = true;
    for (int i = 0; i < CHANNELS; i++)
        last_event[i] = 0;
    init_time = millis();

    this->update();
}

String AudioManager::handleNoteRequest(StaticJsonDocument<200> doc)
{
    bool powered = doc["power"];

    if (powered)
    {
        double frequency = (double)doc["note"];
        tone(OUTPUT_1, frequency);
        return "Success";
    }
    else
    {
        noTone(OUTPUT_1);
        return "Success";
    }

    return "Failure";
}

void AudioManager::stopSong()
{
    this->playing = false;
}

String AudioManager::handleSongRequest(StaticJsonDocument<200> doc)
{
    playSong(doc["id"]);
    return "Success";
}

void AudioManager::init()
{
    pinMode(OUTPUT_1, OUTPUT);
    pinMode(OUTPUT_2, OUTPUT);
    pinMode(OUTPUT_3, OUTPUT);
    pinMode(OUTPUT_4, OUTPUT);

    noTone(OUTPUT_1);
    noTone(OUTPUT_2);
    noTone(OUTPUT_3);
    noTone(OUTPUT_4);
}

void AudioManager::update()
{

    if (!playing)
        return;

    unsigned long curr_time = millis() - init_time;

    for (int i = 0; i < song.get_active_channels(); i++)
    { // i for channels (1 - 4)~

        Track &track = song.get_track(i);
        int cur_ev = last_event[i];

        if (cur_ev >= track.get_no_notes())
            continue;

        // Serial.print("Next timestamp: ");
        // Serial.println(track.get_timestamp(cur_ev));
        // Serial.println(curr_time);

        if (curr_time > track.get_timestamp(cur_ev))
        {
            // Serial.print("Played ");
            // Serial.print(track.get_note(cur_ev));
            // Serial.print(" for ");
            // Serial.print(track.get_duration(cur_ev));
            // Serial.println("ms");
            double freq = noteToFrequency(track.get_note(cur_ev));
            tone(buzzers[i], freq, track.get_duration(cur_ev));
            last_event[i]++;
        }
    }
}