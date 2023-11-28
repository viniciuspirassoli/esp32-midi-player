#include "audio_manager.h"

#include <Arduino.h>
// #include <JsonTranslator.h>
#include <LittleFS.h>
#include <string>

#include "pins.h"

int last_event[CHANNELS];
int buzzers[] = {OUTPUT_1, OUTPUT_2, OUTPUT_3, OUTPUT_4};

uint32_t AudioManager::noteToFrequency(int note)
{
    return uint32_t(440 * exp2((note - 69) / 12.0));
}

AudioManager::AudioManager() {}

bool AudioManager::playSong(int id)
{
    String filename;
    switch (id)
    {
    case 0:
        filename = "/zelda_secret.miniMid";
        break;
    case 1:
        filename = "/sonic_starlight_zone.miniMid";
        break;
    case 2:
        filename = "/smb3_overworld.miniMid";
        break;
    case 3:
        filename = "/smb_overworld.miniMid";
        break;
    case 4:
        filename = "/mii_channel_theme.miniMid";
        break;
    case 5:
        filename = "/zelda_overworld.miniMid";
        break;
    case 6:
        filename = "/careless_whisper.miniMid";
        break;
    case 7:
        filename = "/tetris_theme_a.miniMid";
        break;
    case 8:
        filename = "/scale.miniMid";
        break;
    default:
        playing = false;
        return false;
    }

    if (!this->song.load_from_file(filename))
    {
        playing = false;
        return false;
    }

    this->restartPlayer();

    // this->update();
    return true;
}

void AudioManager::restartPlayer()
{

    playing = true;
    for (int i = 0; i < CHANNELS; i++)
        last_event[i] = 0;
    init_time = millis();
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

    // this->update();
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

    for (int i = 0; i < 4; i++) {
        ledcWriteTone(i+1, 0);
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

void AudioManager::update()
{
    if (!playing)
        return;

    unsigned long curr_time = millis() - init_time;

    for (int i = 0; i < song.get_active_channels(); i++)
    { // i for channels (1 - 4)~
        Track &track = song.get_track(i);
        int cur_ev = last_event[i];

        // Serial.print("Current event: ");
        // Serial.println(cur_ev);

        if (cur_ev >= track.get_number_notes())
        {
            if(song.get_longest_track_id() == i) {
                this->stopSong();
                Serial.println("Stopped song");
            }
            continue;
        }

        // Serial.print("Next timestamp: ");
        // Serial.println(track.get_timestamp(cur_ev));
        // Serial.println(curr_time);

        if (curr_time > track.get_timestamp(cur_ev))
        {
            Serial.print("Played ");
            Serial.print(track.get_note(cur_ev));
            Serial.print(" for ");
            Serial.print(track.get_duration(cur_ev));
            Serial.print("ms");
            uint32_t freq = noteToFrequency(track.get_note(cur_ev));
            Serial.print(" - Frequency: ");
            Serial.println(freq);
            ledcWriteTone(i + 1, freq);
            last_event[i]++;
        }
    }
}

// void AudioManager::update()
// {
//     note_t notes[8] = {NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_B, NOTE_C};

//     int note = 0;

//     while (true)
//     {
//         ledcWriteNote(1, notes[note], 4);
//         ledcWriteNote(2, notes[(note + 2) % 8], 4);
//         ledcWriteNote(3, notes[(note + 4) % 8], 4);
//         ledcWriteNote(4, notes[(note + 6) % 8], 4);

//         vTaskDelay(300 / portTICK_PERIOD_MS);
//         ledcWriteTone(2, 0);
//         ledcWriteTone(1, 0);
//         ledcWriteTone(3, 0);
//         ledcWriteTone(4, 0);
//         vTaskDelay(300 / portTICK_PERIOD_MS);

//         note++;
//         note %= 8;
//     }
// }