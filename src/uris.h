#ifndef URIS_H
#define URIS_H

#include <lv2/atom/atom.h>
#include <lv2/midi/midi.h>
#include <lv2/patch/patch.h>
#include <lv2/urid/urid.h>


typedef struct {
  LV2_URID midi_Event;
} MJURIs;

static inline void map_midijoint_uris(LV2_URID_Map* map, MJURIs* uris) {
  uris->midi_Event         = map->map(map->handle, LV2_MIDI__MidiEvent);
}

#endif /* URIS_H */
