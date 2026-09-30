# Revision 3 hardware

KiCad design files for the integrated EEG/fNIRS main board described in Chapter 3. The July 2022 revision 3 project retains its original filename, `rev2_proto`.

The board contains an STM32H743VIT6, an ESP32-S2-WROVER-I, four ADS1299 converters, power supplies, status indicators and interfaces for microSD storage and external optical hardware. The TLC5940 source driver connects through a daughterboard header. Tethered operation uses an external USB isolator.

## Open the project

Open `rev2_proto.kicad_pro` in KiCad, then open the schematic or PCB editor. The project contains:

- `rev2_proto.kicad_sch`: processors and board interfaces.
- `ADS1299.kicad_sch`, `ADS1299_B.kicad_sch`, `ADS1299_C.kicad_sch`, `ADS1299_D.kicad_sch`: converter sheets.
- `PowerSupplies.kicad_sch` and `Status_LEDs.kicad_sch`.
- `rev2_proto.kicad_pcb`: four-layer PCB layout.
- `Fabrication-2022-07-15/`: 13 Gerber and drill files covering copper, solder mask, paste, silkscreen, outline and plated/non-plated holes.

The project uses KiCad 6 format; the fabrication headers identify KiCad 6.0.4. Opening and exporting the project was also checked with KiCad 10.0.4. The supplied files remain in their original format.

Symbols and footprints are embedded, so the schematic and board can be inspected without restoring workstation-specific libraries. Library-based updates require the corresponding libraries. Optional external 3D models are not included, so the 3D view may be incomplete.

## Revision and use

The selected PCB is the 15 July 2022, 15:24 copy, supplied with the July fabrication outputs. Snapshot hashes and the relationship to the earlier same-day archive are recorded in [PROVENANCE.md](../PROVENANCE.md).

The circuit, layout and fabrication files are unchanged. Preparation removed temporary project files, workstation paths and an obsolete reference URL. A layer-export comparison confirmed unchanged PCB geometry.

This is the main-board project; optical daughterboards, optode assemblies and enclosure parts are separate. A fitted-component BOM is not supplied. Before fabrication, check design rules, component selection, assembly requirements and manufacturing settings. The design is a research prototype, not a clinically certified device.
