#include "audio_manager.h"

#include <Arduino.h>
// #include <JsonTranslator.h>
#include <LittleFS.h>
#include <string>

#include "pins.h"

int last_event[CHANNELS];
int buzzers[] = {OUTPUT_1, OUTPUT_2, OUTPUT_3, OUTPUT_4};

#define NUMBER_OF_MUSICS 3
String songs_list[NUMBER_OF_MUSICS] = {"/careless_whisper.miniMid", "/scale.miniMid", "/smb_overworld.miniMid"};

uint32_t AudioManager::noteToFrequency(int note)
{
    return uint32_t(440 * exp2((note - 69) / 12.0));
}

AudioManager::AudioManager() {}

bool AudioManager::playSong(unsigned int id)
{
    if (id >= NUMBER_OF_MUSICS)
    {
        current_song = 0;
        Serial.println("Invalid id number");
        return false;
    }

    String filename = songs_list[id];
    // String filename = "/careless_whisper.miniMid";

    if (!this->song.load_from_file(filename))
    {
        playing = false;
        return false;
    }

    this->restartPlayer();

    return true;
}

void AudioManager::restartPlayer()
{

    playing = true;
    for (int i = 0; i < CHANNELS; i++)
        last_event[i] = 0;
    init_time = millis();
}

// String AudioManager::handleNoteRequest(StaticJsonDocument<200> doc)
// {
//     bool powered = doc["power"];

//     if (powered)
//     {
//         double frequency = (double)doc["note"];
//         tone(OUTPUT_1, frequency);
//         return "Success";
//     }
//     else
//     {
//         noTone(OUTPUT_1);
//         return "Success";
//     }

//     return "Failure";
// }

void AudioManager::pauseSong()
{
    this->pause_init_time = millis();

    this->playing = false;
    for (int i = 0; i < 4; i++)
    {
        ledcWriteTone(i + 1, 0);
    }
}

void AudioManager::unpauseSong()
{
    unsigned long delta_time = millis() - this->pause_init_time;
    this->init_time += delta_time;
    this->playing = true;
}

void AudioManager::stopSong(bool go_to_next_song)
{
    this->playing = false;

    for (int i = 0; i < 4; i++)
    {
        ledcWriteTone(i + 1, 0);
    }

    if (go_to_next_song)
    {
        Serial.println("going to next song in the end of the music");
        // this->skipSongs(1);
        this->skipSongs(1);
    }
}

String AudioManager::handleSongRequest(StaticJsonDocument<200> doc)
{
    playSong(doc["id"]);
    return "Success";
}

void AudioManager::init()
{
    ledcSetup(1, 8000, 12);
    ledcSetup(2, 8000, 12);
    ledcSetup(3, 8000, 12);
    ledcSetup(4, 8000, 12);

    ledcAttachPin(OUTPUT_1, 1);
    ledcAttachPin(OUTPUT_2, 2);
    ledcAttachPin(OUTPUT_3, 3);
    ledcAttachPin(OUTPUT_4, 4);
}

void AudioManager::skipSongs(int number_of_skips)
{
    current_song = (number_of_skips + current_song) % NUMBER_OF_MUSICS;
    this->playSong(current_song);
}

void AudioManager::update()
{
    if (!playing)
    {
        return;
    }

    unsigned long curr_time = millis() - init_time;

    for (int i = 0; i < song.get_active_channels(); i++)
    {
        // i for channels (1 - 4)~
        update_track(i);
    }
}

void AudioManager::update_track(int track_number)
{
    if (!playing)
    {
        return;
    }

    unsigned long curr_time = millis() - init_time;

    Track &track = song.get_track(track_number);
    int cur_ev = last_event[track_number];

    if (cur_ev >= track.get_number_notes())
    {
        if (song.get_longest_track_id() == track_number)
        {
            this->stopSong(true);
            Serial.println("Stopped song");
        }
        return;
    }

    // Serial.print("Next timestamp: ");
    // Serial.println(track.get_timestamp(cur_ev));
    // Serial.println(curr_time);

    if (curr_time > track.get_timestamp(cur_ev))
    {
        // Serial.print("Played ");
        // Serial.print(track.get_note(cur_ev));
        // Serial.print(" for ");
        // Serial.print(track.get_duration(cur_ev));
        // Serial.print("ms");
        // Serial.print(" - Frequency: ");
        // Serial.println(freq);

        uint32_t freq = noteToFrequency(track.get_note(cur_ev));
        ledcWriteTone(track_number + 1, freq);
        last_event[track_number]++;

        return;
    }

    if ((cur_ev > 0) && (curr_time > track.get_timestamp(cur_ev - 1) + track.get_duration(cur_ev - 1)))
    {
        // stop buzzer
        ledcWriteTone(track_number + 1, 0);
    }
}