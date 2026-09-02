# IRremoteESP8266 for ESPHome / LibreTiny

A build-ready compatibility package based on the official
[IRremoteESP8266 v2.9.0](https://github.com/crankyoldgit/IRremoteESP8266)
release at upstream commit `a0c988c9af5130baae9a0c38dde895e1184b859c`.

IRremoteESP8266 remains responsible for HVAC protocol and state encoding and
decoding. This package adds timing interfaces so ESPHome can perform physical
IR transmit and receive on LibreTiny targets such as BK7231N.

## ESPHome usage

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#esphome-libretiny
```

The tested firmware, MQTT and Tasmota-compatible HTTP interface, and Arabic
usage guide are maintained here:

<https://github.com/zain1144/ESPHome-IRHVAC-BK7231N>

## LibreTiny changes

- Optional mark, space, carrier-frequency, and duty-cycle capture callbacks.
- `irremote_esphome_bridge.h` forwards encoded timings to ESPHome.
- `IRrecv::decodeRaw()` accepts frames captured by ESPHome's
  `remote_receiver` and runs the full protocol decoder.
- The ESP8266/ESP32-specific receiver hardware backend is disabled on
  LibreTiny; ESPHome owns GPIO capture there.
- Sending, decoding, and the common `IRac` HVAC API remain enabled.
- Upstream behavior is unchanged when no capture callbacks are installed.

See [ESPHOME_LIBRETINY.md](ESPHOME_LIBRETINY.md) for technical details.

## Package contents

The repository contains all upstream v2.9.0 library sources under `src/`,
including locale headers, PlatformIO and Arduino metadata, and the original
license. Upstream examples, tests, and extended documentation remain available
in the [original repository](https://github.com/crankyoldgit/IRremoteESP8266).

## Verified build

- ESPHome 2026.8.2
- BK7231N
- LibreTiny 1.12.1
- IRremoteESP8266 2.9.0

## License and attribution

The upstream source remains under its original license and copyright notices.
See [LICENSE.txt](LICENSE.txt). Compatibility additions use the same license.

