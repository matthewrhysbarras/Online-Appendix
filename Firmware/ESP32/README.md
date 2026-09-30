# ESP32-S2 communications firmware

The ESP32-S2 project forwards acquisition packets from the STM32H7 to the Python host receiver over Wi-Fi TCP. It implements the communications path described in Chapter 4; the PC publishes the LSL streams.

Target: ESP32-S2-WROVER-I on the revision 3 main board. Snapshot: 21 March 2022, identified in Appendix B. Original build hashes are listed in [PROVENANCE.md](../../PROVENANCE.md).

## Project layout

- `main/app_main.c`: initialisation, commands and forwarding loop.
- `main/spi.c`, `spi.h`: SPI slave and command handshake.
- `main/wifi.c`, `wifi.h`: access point and TCP server; station-mode functions are retained for reference.
- `main/main.h`: shared declarations.
- `CMakeLists.txt`, `main/CMakeLists.txt`: project and component build inputs.
- `Makefile`, `main/component.mk`: legacy ESP-IDF make entry points.
- `sdkconfig`: study build configuration.

## Configuration and build

The recorded toolchain is ESP-IDF `v4.4-dirty`, target `esp32s2`, with Xtensa GCC 8.4.0 (`esp-2021r2-patch2`). Dependencies include ESP-IDF FreeRTOS, lwIP, Wi-Fi, NVS, SPI slave and TinyUSB. The modified SDK is not included; an exact historical rebuild has not been verified. ESP-IDF 5.4.2 is incompatible with the project's TinyUSB and legacy network APIs.

Set `CONFIGURE_AP_SSID` and `CONFIGURE_AP_PASSWORD` in `main/wifi.h` before building. Use a non-empty password and keep configured credentials out of Git. The application starts in access-point mode on Wi-Fi channel 1, accepts two stations and serves TCP port 80. Configure the host receiver with the access point's address. The station-mode placeholders in `main/wifi.c` apply only if that reference path is used.

With the historical SDK and TinyUSB dependencies restored, the project's CMake workflow is:

```sh
idf.py build
idf.py -p PORT flash monitor
```

Replace `PORT` with the programming connection. This workflow has not been verified on a connected board. The recorded flash settings are DIO, 80 MHz and 2 MB, with bootloader at `0x1000`, partition table at `0x8000` and application at `0x10000`. Use the rebuilt project's flash arguments. The board-specific bootloader-entry procedure is not documented in this package.

## Pins and operation

| Signal | GPIO |
| --- | ---: |
| SPI MOSI / MISO / SCLK / CS | 35 / 37 / 36 / 34 |
| Command-ready handshake | 8 |
| Receive instrumentation | 6 |
| Idle indicator | 16 |

SPI2 runs as a mode-0 slave with a single 2048-byte transaction buffer. The host commands are `Setup_Device`, `Start_Stream` and `STOP!`. Configuration relay carries the host's 42-byte payload in a 44-byte SPI transaction. During streaming, the ESP32 inserts its counter at bytes 1984–1987 and forwards the packet over TCP.

The single-buffer implementation and command handling are retained. Use a controlled local network; TCP backpressure can delay forwarding.

## Preparation changes

Comments were clarified, network credentials replaced with configuration placeholders and the credential-printing startup log removed. Packet logic, pins and timing constants are unchanged. The complete change ledger is in [PROVENANCE.md](../../PROVENANCE.md); existing source notices remain.
