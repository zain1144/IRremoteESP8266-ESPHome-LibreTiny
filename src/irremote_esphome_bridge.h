#pragma once

#include <IRsend.h>

#include "esphome/components/remote_transmitter/remote_transmitter.h"

// Adapter between IRremoteESP8266's protocol encoders and ESPHome's
// platform-safe transmitter. This is especially useful on LibreTiny/Beken,
// where ESP8266-specific bit-banging is not appropriate.
namespace irremote_esphome_bridge {

static esphome::remote_base::RemoteTransmitData *capture_data = nullptr;
static esphome::remote_transmitter::RemoteTransmitterComponent *capture_transmitter = nullptr;

static void capture_mark(uint32_t usec) {
  if (capture_data != nullptr && usec != 0) capture_data->mark(usec);
}

static void capture_space(uint32_t usec) {
  if (capture_data != nullptr && usec != 0) capture_data->space(usec);
}

static void capture_frequency(uint32_t hz, uint8_t duty) {
  if (capture_data != nullptr) capture_data->set_carrier_frequency(hz);
  if (capture_transmitter != nullptr)
    capture_transmitter->set_carrier_duty_percent(duty);
}

static void begin(
    esphome::remote_transmitter::RemoteTransmitterComponent *transmitter,
    esphome::remote_base::RemoteTransmitData *data) {
  capture_transmitter = transmitter;
  capture_data = data;
  irsend_set_capture_callbacks(capture_mark, capture_space, capture_frequency);
}

static void end() {
  irsend_clear_capture_callbacks();
  capture_data = nullptr;
  capture_transmitter = nullptr;
}

}  // namespace irremote_esphome_bridge
