import os
from mido import MidiFile, Message
from mido.midifiles.midifiles import DEFAULT_TEMPO
from mido.midifiles.units import tick2second
import argparse


def isNoteOff(msg: Message) -> bool:
    return msg.type == "note_off" or (msg.type == "note_on" and msg.velocity == 0)


def isNoteOn(msg: Message) -> bool:
    return msg.type == "note_on" and msg.velocity != 0


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="MIDI File Processor")
    parser.add_argument("midi_file", help="Path to the MIDI file")
    args = parser.parse_args()

    file_to_remix = args.midi_file

    mid = MidiFile(file_to_remix)
    filename = os.path.splitext(os.path.basename(file_to_remix))[0]

    tempo = DEFAULT_TEMPO
    song = []
    for i, track in enumerate(mid.tracks):
        notes = []
        stamps = []
        durations = []
        cur_time = 0

        note_duration = 0
        for msg in track:
            delta = tick2second(msg.time, mid.ticks_per_beat, tempo)
            cur_time += delta

            if (msg.is_meta and msg.type == "set_tempo"):
                tempo = msg.tempo

            if (isNoteOn(msg)):
                notes.append(msg.note)
                stamps.append(round(cur_time * 1000))
                note_duration = cur_time
            elif (isNoteOff(msg)):
                durations.append(
                    round((cur_time - note_duration) * 1000 * .95))

        song.append({
            "notes": notes,
            "timestamps": stamps,
            "durations": durations,
            "n_notes": len(notes)
        })

        # print(len(notes), end=' ')
        # print(len(stamps), end=' ')
        # print(len(durations))

    f = open("data/" + filename + ".miniMid", "w")
    f.write(f"{len(song)}\n")

    for track in song:
        f.write(f"{track['n_notes']}\n")
        for note in track["notes"]:
            f.write(f"{note} ")
        f.write("\n")
        for stamp in track["timestamps"]:
            f.write(f"{stamp} ")
        f.write("\n")
        for duration in track["durations"]:
            f.write(f"{duration} ")
        f.write("\n")

    f.close()
