# Study firmware

These are the two processor projects associated with the March 2022 study. The study source is retained, with explicit publication edits; it has not been replaced by later repository versions.

- [STM32](STM32/README.md) controls the four ADS1299 converters, optical sequence and packet construction.
- [ESP32](ESP32/README.md) receives SPI packets and forwards them over a Wi-Fi TCP connection.

The custom Python receiver decodes the data and publishes to LSL on the PC. That receiver is not packaged here: its redistribution and dependencies require separate preparation. The system cannot be reproduced end to end from these two firmware projects alone.

Each packet is 2048 bytes: twenty 99-byte records, a four-byte STM32 counter at offsets 1980–1983, a four-byte ESP32 counter at 1984–1987 and 60 remaining bytes. Each record contains a three-byte optical-state field and 32 three-byte channel words. Numeric field ordering and the fixed optical sequence are preserved.

At a configured raw rate of 1000 samples/s, the twelve-state, five-period optical schedule yields a calculated 1000/60 = 16.67 selected samples/s per decoded optical series. The receiver selects state-tagged records; it does not average the five conversions.

The exact storage-enabled firmware and SD-format example are not recovered. The presence of a microSD interface on the board does not make these source trees a reproduction of every storage demonstration. Later direct-LSL and edge-classification code are separate developments.
