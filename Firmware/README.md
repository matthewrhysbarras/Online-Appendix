# Study firmware

The March 2022 firmware implements the acquisition and streaming path described in Chapter 4.

- [STM32](STM32/README.md): four-ADS1299 acquisition, TLC5940 optical sequencing and packet construction.
- [ESP32](ESP32/README.md): configuration relay and SPI-to-Wi-Fi TCP forwarding.

The separate Python host application decodes the packets and publishes the raw and optical streams to LSL on the PC.

## Data format

Each 2048-byte packet contains twenty 99-byte records, an STM32 counter at bytes 1980–1983 and an ESP32 counter at bytes 1984–1987. Each record contains a three-byte illumination-state field followed by 32 signed three-byte channel values, ordered ADS3, ADS4, ADS2, ADS1. The remaining 60 packet bytes are unassigned. This is the layout in thesis Table 4.4.

The optical schedule cycles through twelve source-wavelength states, with five converter periods per state. At 1000 samples/s, the host selects one tagged record per state to produce 16.67 samples/s per decoded optical series. It does not average the five conversions.

## Scope

These are the study streaming projects. The SD-logging implementation described in the thesis is separate and is not included in these snapshots. Later direct ESP32-to-LSL firmware and the Chapter 6 replay classifier are separate developments.

The component READMEs give build requirements and pin assignments. [PROVENANCE.md](../PROVENANCE.md) records the preparation changes.
