# ESPHome timing bridge: native ESP and LibreTiny

This branch is based on the official IRremoteESP8266 v2.9.0 release at
`8833210f93073b8f732130cf935e18693f93641b`. It keeps IRremoteESP8266
responsible for protocol and HVAC state encoding/decoding and lets ESPHome
perform platform-specific physical transmission and, on LibreTiny, capture.

The original behavior is unchanged when timing callbacks are not installed.
The revision tested by the companion firmware projects is `04b20e7`.

## Exact source changes from upstream v2.9.0

The compatibility patch changes five source files: 167 insertions and two
deletions.

- `src/IRsend.h` exposes optional mark, space, carrier-frequency, and
  duty-cycle callbacks.
- `src/IRsend.cpp` forwards encoded timings to those callbacks when active.
- `src/irremote_esphome_bridge.h` converts the callbacks into ESPHome
  `RemoteTransmitData`.
- `src/IRrecv.h` declares `IRrecv::decodeRaw()` for a caller-provided timing
  buffer.
- `src/IRrecv.cpp` implements `decodeRaw()` and avoids compiling the native
  ESP receiver capture backend when `LIBRETINY` is defined.

No A/C protocol implementation is patched. `IRac.cpp`, `ir_Gree.cpp`,
`ir_Kelvinator.cpp`, and all other `ir_*.cpp` protocol files match upstream
v2.9.0.

## ESPHome usage

Track the compatibility branch:

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#esphome-libretiny
  includes:
    - <irremote_esphome_bridge.h>
```

Pin the tested revision for a reproducible build:

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#04b20e7
```

Complete controllers and standalone YAML configurations are maintained in:

- Native ESP: <https://github.com/zain1144/ESPHome-IRHVAC-ESP>
- BK7231N / LibreTiny:
  <https://github.com/zain1144/ESPHome-IRHVAC-BK7231N>

## Transmit path on all supported targets

IRremoteESP8266's existing `IRac` and protocol classes create the complete
mark/space envelope. The installed callbacks collect those durations in
ESPHome's `RemoteTransmitData`. ESPHome then sends the envelope using its
platform backend:

- ESP32, ESP32-C3, ESP32-S2, and ESP32-S3 use hardware RMT.
- ESP8266 uses ESPHome's software transmitter.
- BK7231N uses the LibreTiny-compatible ESPHome transmitter.

This separation fixed ESP32-C3 frames that were corrupted when direct software
carrier generation was interrupted by Wi-Fi/framework processing. A separate
Tasmota receiver subsequently decoded complete 128-bit Kelvinator and 64-bit
Gree HVAC messages from the installed ESP32-C3 firmware.

## Native ESP receive path

On ESP8266 and ESP32 targets, IRremoteESP8266 retains its normal receive GPIO,
interrupt, timing, and decode path. The `LIBRETINY` guards and external
`decodeRaw()` input are not active there.

## LibreTiny receive path

The upstream receiver's GPIO interrupt/timer backend targets ESP8266 and
ESP32. Therefore, on LibreTiny, ESPHome's `remote_receiver` captures the GPIO
timings and the controller passes each completed frame to the upstream decoder.

`IRrecv::decodeRaw()` expects the same layout used by the native receiver:

- durations are stored in `kRawTick` units;
- index zero is a leading gap;
- marks and spaces alternate after the leading gap; and
- the allocation has one extra element for the decoder's sentinel.

The BK7231N controller converts ESPHome's signed microsecond `RawTimings` to
this layout, calls `decodeRaw()`, and then calls
`IRAcUtils::decodeToState()`. The resulting common HVAC state is serialized in
Tasmota-compatible `IrReceived.IRHVAC` JSON.

`IRrecv::enableIRIn()` intentionally does not attach the native capture
backend on LibreTiny. Do not attach both libraries' interrupts to the same
pin; ESPHome owns capture and passes completed frames to `decodeRaw()`.

## Compatibility verified

- IRremoteESP8266 2.9.0 / compatibility commit `04b20e7`
- ESPHome 2026.8.2
- ESP32-C3: compile, OTA install, and physical send test
- ESP8266: full compile
- BK7231N / LibreTiny 1.12.1: compile, boot, send, and receive paths

LibreTiny 1.12.1 is pinned by the example firmware because it is the runtime
verified to boot reliably on the tested BK7231N device. The library itself is
not tied to that LibreTiny version.

## Updating the base library

Do not replace this dependency directly with the official repository: the
official release does not yet contain the timing bridge or LibreTiny receive
entry point. Merge a newer upstream release into this branch, preserve the
small five-file patch, and rerun native ESP and LibreTiny hardware tests.

PlatformIO caches Git dependencies. After updating
`#esphome-libretiny`, use **Clean Build Files** in ESPHome before compiling.
Pin `#04b20e7` when an identical dependency tree is required.

## Upstream attribution

All upstream source, history, copyright notices, and license terms remain
those of the original IRremoteESP8266 project:

<https://github.com/crankyoldgit/IRremoteESP8266>
