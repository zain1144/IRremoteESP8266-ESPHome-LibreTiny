# ESPHome / LibreTiny timing bridge

This branch is based on the official IRremoteESP8266 v2.9.0 release. It keeps
IRremoteESP8266 responsible for protocol and HVAC state encoding/decoding, but
adds timing interfaces so ESPHome can perform the physical GPIO work.

The original behaviour is unchanged when capture callbacks are not installed.

## Changes from upstream v2.9.0

- `IRsend.h` exposes optional mark, space, and carrier callbacks.
- `IRsend.cpp` sends encoded timings to those callbacks when active.
- `irremote_esphome_bridge.h` forwards the timings to ESPHome's
  `RemoteTransmitterComponent`.
- `IRrecv::decodeRaw()` decodes a caller-provided timing buffer using the full
  upstream protocol decoder.
- On LibreTiny, ESPHome's `remote_receiver` captures the GPIO timings because
  the upstream interrupt/timer backend targets ESP8266/ESP32. The protocol
  decoder itself remains enabled.

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

## LibreTiny receive path

`IRrecv::decodeRaw()` expects the same layout used by the library's native
receiver:

- durations are stored in `kRawTick` units;
- index zero is a leading gap;
- marks and spaces alternate after the leading gap; and
- the allocation has one extra element for the decoder's sentinel.

The complete firmware converts ESPHome's signed microsecond `RawTimings` to
this layout, calls `decodeRaw()`, and then calls
`IRAcUtils::decodeToState()`. This produces the common HVAC state used in
Tasmota's nested `IrReceived.IRHVAC` object.

`IRrecv::enableIRIn()` is intentionally a no-op capture backend on LibreTiny.
Do not attach both IRremoteESP8266 and ESPHome interrupts to the same pin;
ESPHome owns capture and passes completed frames to `decodeRaw()`.

## Compatibility

The branch has been compiled with:

- IRremoteESP8266 2.9.0
- ESPHome 2026.8.2
- BK7231N / LibreTiny 1.12.1

LibreTiny 1.12.1 is used by the example firmware because it is the last runtime
verified to boot reliably on the tested device. The library itself is not tied
to that LibreTiny version.

## Upstream attribution

All upstream source, history, copyright notices, and license terms remain those
of the original IRremoteESP8266 project:

<https://github.com/crankyoldgit/IRremoteESP8266>
