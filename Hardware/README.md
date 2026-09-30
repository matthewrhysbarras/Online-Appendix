# Revision 3 hardware

This directory contains the latest verified design representation of the submitted platform: the July 2022 revision 3 KiCad files, retaining the historical project name `rev2_proto`. The main board integrates an STM32H743VIT6, an ESP32-S2-WROVER-I and four ADS1299 converters, together with power supplies, status indicators and interfaces for external optical hardware and microSD storage.

The selected PCB is the preserved **15 July 2022, 15:24:40** copy, SHA-256 `4037a89526a83346fe1681b8de66f691b875b99b842814eeb6874a4de77ba51e` before sanitisation. The independently verified revision 3 source ZIP contains an earlier **14:37:12** board. The later copy changes silkscreen positions/text (including RST/BOOT and JUL 22 labels) and the stored precision of four rounded-pad ratios; the seven schematic files and project are text-identical after line-ending normalisation. These are preserved historical differences, not edits made for publication. The earlier ZIP remains private reference provenance.

Matthew confirmed the design identity with the integrated board used for the March 2022 recordings. These July files do not establish the exact March fabrication revision or fitted component population. This release does not substitute an unrelated later board design.

## Opening the project

Open `rev2_proto.kicad_pro` in KiCad, then open its schematic and PCB. The retained schematic hierarchy consists of:

- `rev2_proto.kicad_sch`: processor and board interfaces.
- `ADS1299.kicad_sch`, `ADS1299_B.kicad_sch`, `ADS1299_C.kicad_sch`, `ADS1299_D.kicad_sch`: converter sheets.
- `PowerSupplies.kicad_sch` and `Status_LEDs.kicad_sch`.
- `rev2_proto.kicad_pcb`: editable four-layer PCB.

The files use the KiCad 6 format. The preserved July Gerber and drill headers identify **KiCad 6.0.4**. This establishes the fabrication-output version, not the version used for every earlier edit. KiCad 10.0.4 successfully loaded this candidate and exported the schematic hierarchy, netlist and PCB layers during release preparation. It was not used to save or migrate the supplied design files.

Symbols are embedded in the modern schematic files and footprints in the PCB. Machine-specific footprint-library tables and legacy rescue/cache libraries are excluded. The embedded design remains inspectable and editable, but library-based symbol/footprint replacement or updating requires restoring appropriate libraries and checking their redistribution terms. Do not update footprints or regenerate the board as a prerequisite to opening it.

Two unavailable 3D-model references containing workstation paths were removed. Standard KiCad 3D-library references remain; an incomplete 3D view is possible. Separate downloaded models and loose third-party libraries are not included. Existing notices and attribution are summarised in [licensing](../LICENSING.md).

A dead external download URL was removed from footprint descriptions; the IPC document and page attribution remain. Other vendor links are historical reference pointers, not bundled dependencies.

## Fabrication outputs and status

`Fabrication-2022-07-15/` contains 13 preserved files: four copper layers, front/back solder mask, front/back paste, front/back silkscreen, board outline and plated/non-plated drill files. They are byte-identical to both the separate July archive and its loose preserved outputs. No new production files have replaced them.

The original revision 3 source ZIP also contained a **November 2021** Gerber directory. That earlier output set is excluded to avoid presenting it as July fabrication data. The July outputs were recovered separately beside the selected 15:24 KiCad board; their creation timestamps immediately follow that saved PCB. Their shared project identity, dates and source location support this association; no historical job submission or manufacturer acceptance record was recovered. A same-tool export comparison of the selected original and cleaned PCB checks that publication cleanup preserves layer geometry; it is not a bit-for-bit re-creation of KiCad 6.0.4 fabrication output.

No circuit, connectivity, footprint placement, track, zone, pad, drill or silkscreen has been intentionally changed. No new electrical-rule or design-rule fixes were applied. The package is not a manufacturing sign-off: check fabrication settings, design rules, component availability, assembly/BOM and board bring-up before ordering. No verified fitted-population BOM is supplied.

The TLC5940 connects through an external-board header; the revision 3 main-board archive does not supply a complete optical/mechanical assembly. Earlier daughterboard and enclosure designs remain separate evidence. The final USB connection used an external isolated cable according to the author; this CAD does not implement an on-board ADuM4160 isolator. A legacy isolator sheet is not part of the active hierarchy and is excluded.

The design has no claimed clinical validation, comprehensive ambulatory validation or regulatory readiness. See [provenance](../PROVENANCE.md) and [review items](../REVIEW.md).
