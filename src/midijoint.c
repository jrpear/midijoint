#include <stdlib.h>

#include <lv2/core/lv2.h>
#include <lv2/core/lv2_util.h>
#include <lv2/atom/atom.h>
#include <lv2/atom/util.h>
#include <lv2/urid/urid.h>
#include <lv2/log/log.h>
#include <lv2/log/logger.h>

#include "uris.h"

#define MIDIJOINT_URI "https://github.com/jrpear/midijoint"

typedef enum { MIDI_IN = 0, MIDI_OUT = 1 } MJPortsIdx;

typedef struct {
  const LV2_Atom_Sequence *midi_in;
  LV2_Atom_Sequence *midi_out;
} MJPorts;

typedef struct {
  LV2_URID_Map *map_data;
  LV2_Log_Logger logger;
  MJPorts ports;
  MJURIs uris;
} Midijoint;

static void connect_port(LV2_Handle instance, uint32_t port, void *data) {
  Midijoint *self = instance;
  switch (port) {
  case MIDI_IN:
    self->ports.midi_in = data;
    break;
  case MIDI_OUT:
    self->ports.midi_out = data;
    break;
  }
}

static LV2_Handle instantiate(const LV2_Descriptor *descriptor, double rate,
                              const char *path,
                              const LV2_Feature *const *features) {
  Midijoint *self = malloc(sizeof(Midijoint));
  if (!self) {
    return NULL;
  }

  const char *missing =
      lv2_features_query(features, LV2_URID__map, &self->map_data, true,
          LV2_LOG__log, &self->logger.log, true, NULL);

  lv2_log_logger_set_map(&self->logger, self->map_data);

  if (missing) {
    lv2_log_error(&self->logger, "Missing feature <%s>\n", missing);
    free(self);
    return NULL;
  }

  map_midijoint_uris(self->map_data, &self->uris);

  return (LV2_Handle)self;
}

static void cleanup(LV2_Handle instance) { free(instance); }

static void run(LV2_Handle instance, uint32_t sample_count) {
  Midijoint *self = instance;
  const uint32_t out_capacity = self->ports.midi_out->atom.size;

  lv2_atom_sequence_clear(self->ports.midi_out);
  self->ports.midi_out->atom.type = self->ports.midi_in->atom.type;

  LV2_ATOM_SEQUENCE_FOREACH(self->ports.midi_in, /*LV2_Atom_Event*/ ev) {
    lv2_atom_sequence_append_event(self->ports.midi_out, out_capacity, ev);
  }
}

static const LV2_Descriptor descriptor = {
    MIDIJOINT_URI, instantiate, connect_port,
    NULL, // activate,
    run,
    NULL, // deactivate,
    cleanup,
    NULL // extension_data
};

const LV2_Descriptor *lv2_descriptor(uint32_t index) {
  return index == 0 ? &descriptor : NULL;
}
