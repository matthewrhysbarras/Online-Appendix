# Review status and unresolved items

This collection is prepared locally for **private online reviewer access**, separate from the thesis itself. No upload or live website change has been performed.

## Verification

- Compared all 137 copied source, dependency, project, build-recipe and fabrication files with originals: 128 unchanged and nine with declared publication transformations.
- C/H executable-token comparisons pass after the explicit allowlist of identifier/network-safety changes. Historical STM32 build-source completeness and schematic hierarchy checks pass.
- The revision 3 source ZIP matches an independently downloaded private-repository copy by SHA-256. All 13 July fabrication files match both their preserved ZIP members and loose copies.
- The primary PCB is the later preserved 15:24 copy, not the 14:37 board inside that ZIP. Historical silkscreen and pad-ratio precision differences are recorded in the provenance notes. No electrical redesign was introduced during preparation.
- KiCad 10.0.4 exports the seven-sheet schematic PDF, netlist and PCB layers. No design files were resaved.
- All 22 fresh PCB layer exports and the Gerber job record match exports of the preserved original after excluding generated timestamps. This checks publication geometry preservation, not manufacturing acceptance.
- STM32: all historical C and assembly inputs compile with available GNU Arm 11.3.1; full linking fails on the historical duplicate `ADS_Buffer` definitions. Original compiler was GNU Tools for STM32 9.3.1. No compatibility repair was applied.
- ESP32: attempted build in available ESP-IDF v5.4.2 fails on missing historical TinyUSB and removed legacy network/event APIs. Original metadata says `v4.4-dirty`; those SDK modifications remain unidentified. No API migration was applied.
- No flashing, device run, timing measurement or manufacturing validation was performed.

External-link checking found a dead download URL in footprint descriptions; it was removed while retaining the document/page attribution. The platform DOI and one Hirose datasheet endpoint returned access-denied responses, and the intended thesis landing page and a Molex endpoint could not be verified. These are recorded as access/verification limits, not assumed to be missing publications. The edge-replay DOI resolved successfully. Relative documentation links are checked locally.

## Reviewer access and reproduction notes

1. Confirm the private hosting destination and reviewer access controls before an upload. Selecting public-release licences is not a prerequisite for this preparation task; existing notices and attribution are retained.
2. Review the redaction and startup-log change. Rotate any historical network secrets still in use; they remain in the private originals and may remain in old binaries.
3. Exact ESP-IDF v4.4 modifications and historical compiler environments remain necessary for a verified historical rebuild. These gaps do not prevent source inspection. Review existing defects separately from the study-preserving copy.
4. The recorded thesis title identifies the accompanying work. No thesis DOI or public publication status is invented, and establishing either is not a prerequisite for this private handoff.
5. The companion Python receiver is a separate dependency for end-to-end reproduction. Exact storage-enabled source, fitted-population BOM, full external optical assembly and March manufacturing files remain unresolved.

The collection deliberately preserves historical limitations. Any functional corrections should be a separately named development release with its own validation, not silently substituted for the evaluated snapshots.
