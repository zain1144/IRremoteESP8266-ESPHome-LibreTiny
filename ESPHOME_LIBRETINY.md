# ESPHome / LibreTiny timing bridge

This branch is based on the official IRremoteESP8266 v2.9.0 release. It keeps
IRremoteESP8266 responsible for protocol and HVAC state encoding, but adds an
optional timing-capture interface so ESPHome can perform the physical output.

The original behaviour is unchanged when capture callbacks are not installed.

## Changes from upstream v2.9.0

- `IRsend.h` exposes optional mark, space, and carrier callbacks.
- `IRsend.cpp` sends encoded timings to those callbacks when active.
- `irremote_esphome_bridge.h` forwards the timings to ESPHome's
  `RemoteTransmitterComponent`.
- `IRrecv.cpp` is excluded on LibreTiny because the upstream receiver timer
  implementation targets ESP8266/ESP32. Sending and `IRac` remain available.

## ESPHome usage

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#esphome-libretiny
  includes:
    - <irremote_esphome_bridge.h>
```

The complete BK7231N YAML, Tasmota-compatible MQTT/HTTP controller, and Arabic
documentation are maintained at:

<https://github.com/zain1144/ESPHome-IRHVAC-BK7231N>

## Compatibility

The branch has been compiled with:

- IRremoteESP8266 2.9.0
- ESPHome 2026.8.2
- BK7231N / LibreTiny 1.12.1 and 1.13.0

LibreTiny 1.12.1 is used by the example firmware because it is the last runtime
verified to boot reliably on the tested device. The library itself is not tied
to that LibreTiny version.

## Upstream attribution

All upstream source, history, copyright notices, and license terms remain those
of the original IRremoteESP8266 project:

<https://github.com/crankyoldgit/IRremoteESP8266>
