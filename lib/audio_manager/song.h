#ifndef SONG_HEADER
#define SONG_HEADER

#define CHANNELS 4
#define MAX_PER_CHANNEL 300

#include <LittleFS.h>

class Song;

class Track
{
private:
    int n_notes;
    int notes[MAX_PER_CHANNEL];
    unsigned int timestamps[MAX_PER_CHANNEL], durations[MAX_PER_CHANNEL];

public:
    Track()
    {
        this->n_notes = 0;
    }

    int load_from_file(File &file);
    void load_from_values(int notes[], unsigned int timestamps[], unsigned int durations[], int n_notes);

    int get_note(int index) { return this->notes[index]; };
    unsigned int get_timestamp(int index) { return this->timestamps[index]; };
    unsigned int get_duration(int index) { return this->durations[index]; };
    int get_number_notes() { return this->n_notes; };
    int get_track_duration() { return this->durations[this->n_notes - 1] + this->timestamps[this->n_notes - 1]; };

    // void set_note(int index, int note) {this->notes[index] = note;};
    // void set_timestamp(int index, unsigned int timestamp) {this->timestamps[index] = timestamp;};
    // void set_duration(int index, unsigned int duration) {this->durations[index] = duration;};
    // void set_no_notes(int no_notes) {this->n_notes = n_notes;};
};

class Song
{
private:
    Track tracks[CHANNELS];
    int longest_track_id = 0;
    String name;

public:
    int n_tracks = 0, active_channels = 0;
    Song(){};
    Song(int n_tracks)
    {
        this->n_tracks = n_tracks;
        active_channels = min(CHANNELS, n_tracks);
    };

    bool load_from_file(String &filename);

    int get_n_tracks() { return n_tracks; };
    int get_active_channels() { return n_tracks; };
    Track &get_track(int index) { return tracks[index]; };

    String getName() { return name; };

    void serial_print();
    int get_longest_track_id();
    int get_song_duration() {
        return tracks[this->get_longest_track_id()].get_track_duration() / 1000;
    }
};

#endif