# IRremoteESP8266 timing bridge for ESPHome and LibreTiny

A build-ready compatibility package based on the official
[IRremoteESP8266 v2.9.0](https://github.com/crankyoldgit/IRremoteESP8266/releases/tag/v2.9.0)
release at upstream commit `8833210f93073b8f732130cf935e18693f93641b`.
The tested compatibility revision is `04b20e7`.

IRremoteESP8266 remains responsible for A/C protocol encoding, decoding, and
common `stdAc` state conversion. This branch adds small timing interfaces so
ESPHome can perform the physical transmit or receive work appropriate for the
target platform.

It is used by both projects below:

- [ESPHome-IRHVAC-ESP](https://github.com/zain1144/ESPHome-IRHVAC-ESP) for
  ESP8266, ESP32, ESP32-C3, ESP32-S2, and ESP32-S3.
- [ESPHome-IRHVAC-BK7231N](https://github.com/zain1144/ESPHome-IRHVAC-BK7231N)
  for BK7231N / LibreTiny.

## Why the fork is also used on native ESP

The repository name mentions LibreTiny because that was the original target,
but the transmit timing callbacks are platform-independent. On ESP32 they let
IRremoteESP8266 encode the message while ESPHome's hardware RMT peripheral
emits the carrier and timing envelope. This avoids the malformed 80-270 us
pulses observed when direct software carrier generation on ESP32-C3 was
interrupted by Wi-Fi or framework work.

On native ESP, reception still uses IRremoteESP8266's normal receiver backend.
The external raw-capture path and receiver backend guards are compiled only
when `LIBRETINY` is defined.

## ESPHome usage

To track the maintained compatibility branch:

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#esphome-libretiny
```

To reproduce the hardware-tested build exactly:

```yaml
esphome:
  libraries:
    - IRremoteESP8266=https://github.com/zain1144/IRremoteESP8266-ESPHome-LibreTiny.git#04b20e7
```

The two complete firmware projects contain the required controller, MQTT
topics, Tasmota-compatible `IRHVAC` and `IrReceived` JSON, full YAML files, and
Arabic and English instructions.

## Exact changes from upstream v2.9.0

The source patch is deliberately small: five files, 167 insertions, and two
deletions compared with the official `v2.9.0` tag.

| File | Change | Used by |
|---|---|---|
| `src/IRsend.h` | Optional mark, space, carrier-frequency, and duty-cycle callback API | ESP and LibreTiny transmit |
| `src/IRsend.cpp` | Sends the encoded envelope to callbacks when active; preserves the original path otherwise | ESP and LibreTiny transmit |
| `src/irremote_esphome_bridge.h` | Converts callbacks into ESPHome `RemoteTransmitData` | ESP and LibreTiny transmit |
| `src/IRrecv.h` | Adds the caller-provided raw timing decode interface | LibreTiny receive |
| `src/IRrecv.cpp` | Implements `decodeRaw()` and guards the native capture backend on `LIBRETINY` | LibreTiny receive |

No protocol implementation was modified. In particular, `IRac.cpp`,
`ir_Gree.cpp`, `ir_Kelvinator.cpp`, and all other protocol encoder/decoder
files are unchanged from upstream v2.9.0. Protocol support therefore remains
the support supplied by upstream IRremoteESP8266.

See [ESPHOME_LIBRETINY.md](ESPHOME_LIBRETINY.md) for the capture-buffer layout
and platform-specific data paths.

## Platform data paths

### ESP8266 and ESP32

IRremoteESP8266 creates the complete protocol envelope. The bridge stores its
mark/space durations in `RemoteTransmitData`, then ESPHome transmits them. The
ESP32 family uses hardware RMT; ESP8266 uses ESPHome's software transmitter.
IRremoteESP8266 continues to own the receiver GPIO and decode captured frames
directly.

### BK7231N / LibreTiny

Transmission uses the same callback bridge. For reception, ESPHome's
`remote_receiver` owns GPIO capture, the firmware converts its signed
microsecond durations to IRremoteESP8266's raw-buffer layout, and
`IRrecv::decodeRaw()` runs the normal protocol decoder.
`IRAcUtils::decodeToState()` then produces the common HVAC state used in
`IrReceived.IRHVAC`.

## Verified combinations

- ESPHome `2026.8.2` with IRremoteESP8266 `2.9.0` / bridge `04b20e7`.
- ESP32-C3: full compile, OTA installation, and real end-to-end transmission.
  A separate Tasmota receiver decoded complete 128-bit Kelvinator and 64-bit
  Gree HVAC states.
- ESP8266: full firmware compile, covering ESPHome's software transmit path.
- ESP32, ESP32-S2, and ESP32-S3: YAML configuration validation; they use the
  same ESP32 RMT path but were not part of the physical-device test.
- BK7231N with LibreTiny `1.12.1`: firmware compile, boot, transmission, and
  reception with the external ESPHome capture path.

These results validate the integration paths, not every model or feature of
every A/C protocol.

## Updating from upstream

The unmodified official library is not currently a drop-in replacement for
these projects because it does not contain the timing callback bridge or the
LibreTiny raw-capture interface. To adopt a newer IRremoteESP8266 release:

1. Merge or rebase that upstream release into `esphome-libretiny`.
2. Resolve the five-file compatibility patch, keeping protocol files
   unchanged whenever possible.
3. Compile the ESP8266, ESP32-C3, and BK7231N reference configurations.
4. Test Gree and Kelvinator transmission with an independent receiver and test
   LibreTiny reception on hardware.
5. Move the tested commit pin only after those checks pass.

PlatformIO caches Git dependencies and does not update them on every compile.
When following `#esphome-libretiny`, run **Clean Build Files** in ESPHome after
the branch changes. Use commit `#04b20e7` instead when reproducibility is more
important than automatically following the compatibility branch.

## Package contents

The repository contains the upstream v2.9.0 library sources under `src/`,
including locale headers, PlatformIO and Arduino metadata, and the original
license. Upstream examples, tests, and extended documentation remain in the
[original repository](https://github.com/crankyoldgit/IRremoteESP8266).

## License and attribution

The upstream source remains under its original license and copyright notices.
See [LICENSE.txt](LICENSE.txt). Compatibility additions use the same license.
