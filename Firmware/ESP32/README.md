# ESP32-S2 study communications firmware

Target: ESP32-S2, on the integrated instrument represented by the ESP32-S2-WROVER-I revision 3 CAD. Purpose: relay host configuration to the STM32 and forward acquisition packets over IPv4 TCP. This is the Chapter 4 study route, with PC-side LSL publication.

## Provenance and layout

The preserved project contains a build dated 21 March 2022. Its original application binary SHA-256 is `a46ada9cb61cc105341fa3b6d3193f429d0d66189247051dc5a8554155196353`; that binary is retained privately because compiled strings may contain credentials. No source commit is established for this snapshot. See [provenance](../../PROVENANCE.md).

- `main/app_main.c`: initialisation, host commands and main forwarding loop.
- `main/spi.c`, `spi.h`: SPI slave buffers, transactions and command handshake.
- `main/wifi.c`, `wifi.h`: access point, TCP server and retained station-mode reference functions.
- `main/main.h`: shared declarations.
- `CMakeLists.txt`, `main/CMakeLists.txt`: CMake project and component inputs.
- `Makefile`, `main/component.mk`: preserved legacy ESP-IDF make entry points, not separately verified.
- `sdkconfig`: preserved study build settings.

## Toolchain, configuration and build

The build metadata identifies **ESP-IDF `v4.4-dirty`**, target `esp32s2`, and the Xtensa ESP32-S2 GCC **8.4.0, esp-2021r2-patch2** toolchain. The local changes behind `dirty` have not been recovered. This is therefore not a fully pinned or bit-for-bit reproducible SDK environment. Dependencies include ESP-IDF FreeRTOS, lwIP, Wi-Fi, NVS, SPI slave and the historical TinyUSB component/API. The SDK is not vendored here.

In `main/wifi.h`, replace `CONFIGURE_AP_SSID` and `CONFIGURE_AP_PASSWORD` with local settings before building. The shipped strings are redaction placeholders, not secure credentials. Keep the password non-empty to avoid the historical open-access-point branch. If deliberately using the retained station-mode function, separately configure its two placeholders in `main/wifi.c`; the study application starts access-point mode. Keep locally configured copies private. No private IP address is supplied; the PC must use the address assigned to the access point in its own configuration. TCP port 80, Wi-Fi channel 1 and the two-station limit are unchanged protocol settings.

The following is the expected CMake workflow from the preserved project, conditional on restoring the historical SDK and TinyUSB API. It is **not a verified fresh build recipe**:

```sh
# Activate the recovered ESP-IDF v4.4 environment, then enter this directory.
idf.py build
idf.py -p PORT flash monitor
```

Replace `PORT` with the actual programming connection. The historical flash arguments specify DIO, 80 MHz and 2 MB, with bootloader at `0x1000`, partition table at `0x8000` and application at `0x10000`. Use the rebuilt project's flash arguments, not a standalone application binary. The exact board bootloader-entry and physical programming procedure have not been verified, and no flashing was performed during release preparation.

An attempted build with available ESP-IDF v5.4.2 failed because `tinyusb.h` and legacy networking/event APIs are absent. This different environment is not evidence of a historical rebuild. Do not regenerate `sdkconfig`, migrate APIs or change target to make this candidate build without recording a separate development version.

## Pins and flow

| Signal | ESP32-S2 GPIO |
| --- | ---: |
| SPI MOSI, MISO, SCLK, CS | 35, 37, 36, 34 |
| Command-ready handshake to STM32 | 8 |
| Historical receive instrumentation output | 6 |
| Idle indicator output | 16 |

SPI2 operates as a mode-0 slave, with 2048-byte buffers and queue depth one. `Setup_Device` introduces the host configuration; `Start_Stream` starts acquisition; `STOP!` requests stop. Configuration relay uses 44 SPI bytes for the host's 42-byte payload. The main loop receives a packet, inserts the ESP32 counter at offsets 1984–1987, and calls the TCP sender. The custom PC application performs decoding and LSL publication.

## Limitations and publication edits

Single-buffer reception, historical timeout/error handling, command parsing and concurrency are preserved. This code has not been hardened for hostile network clients. A successful TCP send does not prove complete recording. No contemporary hardware, throughput or long-duration test was performed.

Publication edits clarify comments, remove commented-out alternative implementations, redact access-point and station credentials, remove an unused private-address macro, and remove the log call that printed credentials. Removing that call changes startup diagnostic output and its execution time; this is an explicit safety change. Acquisition code, packet values, pin assignments and timing constants are unchanged. Other active diagnostics are retained because removing them would change historical execution timing. Copyright and example-code notices remain in place; see [licensing](../../LICENSING.md).
