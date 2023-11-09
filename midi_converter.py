from os import write
from mido import MidiFile, Message
from mido.midifiles.midifiles import DEFAULT_TEMPO
from mido.midifiles.units import tick2second

import math

# file_to_remix = "sonic_starlight_zone"
# file_to_remix = "zelda_secret"
# file_to_remix = "smb3_overworld"
# file_to_remix = "smb_medley"
file_to_remix = "smb_overworld"
# file_to_remix = "mii_channel_theme"
# file_to_remix = "amogus_sus"
# file_to_remix = "careless_whisper"
# file_to_remix = "zelda_overworld"
# file_to_remix = "tetris_theme_a"


mid = MidiFile('songs/' + file_to_remix + ".mid")

def isNoteOff(msg: Message) -> bool:
    return msg.type == "note_off" or (msg.type == "note_on" and msg.velocity == 0)

def isNoteOn(msg: Message) -> bool:
    return msg.type == "note_on" and msg.velocity != 0


tempo = DEFAULT_TEMPO
song = []
for i, track in enumerate(mid.tracks):
    notes = []
    stamps = []
    durations = []
    cur_time = 0

    # print('Track {}: {}'.format(i, track.name))
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
            durations.append(round((cur_time - note_duration) * 1000 * .95))

    song.append({
        "notes": notes,
        "timestamps": stamps,
        "durations": durations,
        "n_notes": len(notes)
    })
    
    print(len(notes), end=' ')
    print(len(stamps), end=' ')
    print(len(durations))

# import json
f = open("data/" + file_to_remix + ".miniMid", "w")
# json.dump(song, f)
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