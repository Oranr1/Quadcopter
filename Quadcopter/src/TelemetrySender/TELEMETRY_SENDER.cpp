#include "TELEMETRY_SENDER_exports.h"

TELEMETRY_SENDER_t telemetry_sender__telemetry_g;

void TELEMETRY_SENDER__send(TELEMETRY_SENDER_id_t id, void *payload)
{
  telemetry_sender__telemetry_g.id = id;
#define ID_TO_FIELD(__id, __type) \
  if (__id == id) { \
    memcpy(&telemetry_sender__telemetry_g.payload, \
           payload, \
           sizeof(__type)); \
  }
#include "TELEMETRY_SENDER_id_to_field.hx"
#undef ID_TO_FIELD
  myRadio.stopListening();
  myRadio.write(&telemetry_sender__telemetry_g, sizeof(telemetry_sender__telemetry_g));
  myRadio.startListening();
}
