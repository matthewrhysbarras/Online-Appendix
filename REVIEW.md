# Verification summary

Checks completed during preparation on 30 September 2026:

| Check | Result |
| --- | --- |
| Comparison with original files | 137 files checked: 128 identical, nine with documented preparation changes |
| Firmware source comparison | Executable-token checks pass with the changes listed in [PROVENANCE.md](PROVENANCE.md) |
| Build-input inventory | STM32 link inputs and ESP32 application inputs present |
| Hardware identity | Original design archive independently matched by SHA-256; all 13 fabrication files matched their originals |
| KiCad project | Seven-sheet schematic, netlist and PCB layers exported successfully |
| PCB geometry | All 22 layer exports and the Gerber job match the selected original, excluding generated timestamps |
| Package checks | File scans, archive integrity, relative documentation links and SHA-256 checks pass |

## Build checks

The STM32 sources compile with GNU Arm 11.3.1; linking reports duplicate `ADS_Buffer` definitions. The recorded study compiler is GCC 9.3.1.

The ESP32 project requires the historical ESP-IDF `v4.4-dirty` environment. ESP-IDF 5.4.2 does not provide its TinyUSB and legacy network APIs.

Historical-toolchain rebuilds and device flashing were not performed. See the component READMEs for the recorded toolchains and build procedures.
