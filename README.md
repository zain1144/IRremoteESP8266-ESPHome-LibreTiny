# IRremoteESP8266 for ESPHome / LibreTiny

A build-ready compatibility package based on the official
[IRremoteESP8266 v2.9.0](https://github.com/crankyoldgit/IRremoteESP8266)
release (upstream commit `a0c988c9af5130baae9a0c38dde895e1184b859c`).

It keeps IRremoteESP8266 responsible for HVAC protocol/state encoding and adds
an optional timing bridge so ESPHome's `remote_transmitter` can perform the
physical IR output on LibreTiny targets such as BK7231N.

## ESPHome usage

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#esphome-libretiny
```

The complete tested firmware, Tasmota-compatible MQTT/HTTP interface, Home
Assistant example, and Arabic guide are here:

<https://github.com/zain1144/ESPHome-IRHVAC-BK7231N>

## LibreTiny changes

- Optional mark, space, carrier-frequency, and duty-cycle capture callbacks.
- `irremote_esphome_bridge.h` forwards encoded timings to ESPHome.
- The ESP8266/ESP32-specific receiver implementation is excluded on LibreTiny.
- Sending and the common `IRac` HVAC API remain enabled.
- Upstream behavior is unchanged when no capture callbacks are installed.

See [ESPHOME_LIBRETINY.md](ESPHOME_LIBRETINY.md) for technical details.

## Package contents

This repository contains all upstream v2.9.0 library sources under `src/`,
including its locale headers, plus the PlatformIO/Arduino metadata and license
required for direct installation. Upstream examples, tests, and extended
documentation remain available in the
[original repository](https://github.com/crankyoldgit/IRremoteESP8266).

## Verified build

Compiled successfully with:

- ESPHome 2026.8.2
- BK7231N
- LibreTiny 1.12.1
- IRremoteESP8266 2.9.0

## License and attribution

The upstream source remains under its original license and copyright notices.
See [LICENSE.txt](LICENSE.txt). The compatibility additions are provided under
the same license.
