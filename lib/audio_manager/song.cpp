#include "song.h"

#include <Arduino.h>
#include <LittleFS.h>

void Track::load_from_file(File &file)
{
    n_notes = file.parseInt();

    for (int i = 0; i < n_notes; i++)
    {
        notes[i] = file.parseInt();
    }
    for (int i = 0; i < n_notes; i++)
    {
        timestamps[i] = (unsigned int)file.parseInt();
    }
    for (int i = 0; i < n_notes; i++)
    {
        durations[i] = (unsigned int)file.parseInt();
    }
}

void Track::load_from_values(int notes[], unsigned int timestamps[], unsigned int durations[], int n_notes)
{
    this->n_notes = n_notes;
    notes[0] = 150;
    notes[1] = 150;
    notes[2] = 150;

    this->timestamps[0] = 0;
    this->timestamps[1] = 100;
    this->timestamps[2] = 200;

    this->durations[0] = 50;
    this->durations[1] = 50;
    this->durations[2] = 50;
}

bool Song::load_from_file(String &filename)
{

    File file = LittleFS.open(filename, "r");
    if (!file)
    {
        Serial.println("Failed to open file: " + filename);
        return false;
    }
    Serial.println("Opened " + filename + " (" + file.size() + ")");

    this->n_tracks = file.parseInt();
    this->active_channels = min(CHANNELS, n_tracks);

    Serial.print("Tracks: ");
    Serial.println(n_tracks);

    for (int i = 0; i < n_tracks; i++)
    {
        Track &loop_track = this->get_track(i);
        loop_track.load_from_file(file);

        Serial.print("Track ");
        Serial.print(i);
        Serial.print(" has ");
        Serial.print(loop_track.get_no_notes());
        Serial.println(" notes");
    }

    file.close();

    return true;
}

void Song::serial_print()
{
    for (int i = 0; i < active_channels; i++)
    {
        Track &t = get_track(i);
        Serial.print("Channel ");
        Serial.println(i);
        for (int j = 0; j < t.get_no_notes(); j++)
        {
            Serial.print(t.get_note(j));
            Serial.print(" ");
        }
        Serial.println();
    }
}