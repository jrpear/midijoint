#ifndef MSGREL_H
#define MSGREL_H

#include <lv2/midi/midi.h>
#include <stdint.h>

#define REL_SIZE 16

typedef struct {
  uint8_t statchan;
  uint8_t data[2];
} MIDIMsg;

typedef struct {
  LV2_Atom_Event ev;
  MIDIMsg msg;
} MIDIMsgEv;

// Specifies a single message
typedef struct {
  uint8_t statchan; // aaaabbbb where a is status (opcode) and b is channel
  uint8_t data;     // ignores vel byte on note messages
} VMSpec;           // Voice Message Specifier

bool MIDI_match(const MIDIMsg *msg_real, const VMSpec *msg_spec) {
  return msg_real->statchan == msg_spec->statchan &&
         msg_real->data[0] == msg_spec->data;
}

typedef struct {
  bool active;
  VMSpec in;
  VMSpec out;
} VMPair; // Voice Message Pair

typedef struct {
  VMPair pairs[REL_SIZE];
} VMBinRel;

#endif // MSGREL_H
