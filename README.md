# PhD research artefacts

Research firmware and hardware accompanying Matthew Rhys Barras's University of Hull PhD thesis, *Toward Mobile Neuroimaging: Design and Implementation of a Real-Time Multi-Modal EEG/fNIRS System*.

This collection is prepared for **restricted online access by the reviewer**, not public release or insertion into the thesis. It contains cleaned copies of the study artefacts, with the provenance and changes described in [PROVENANCE.md](PROVENANCE.md). Preserved originals remain unchanged in the private evidence archive.

| Directory | Contents and version |
| --- | --- |
| [Firmware/ESP32](Firmware/ESP32/README.md) | ESP32-S2 communications source associated with the 21 March 2022 build |
| [Firmware/STM32](Firmware/STM32/README.md) | STM32H743 acquisition source associated with the 30 March 2022 Release build, vendor dependencies and historical build recipe |
| [Hardware](Hardware/README.md) | Revision 3 KiCad project, July 2022, and separately preserved July fabrication outputs |

The evaluated communication path is STM32 acquisition → ESP32-S2 SPI-to-TCP forwarding → custom Python receiver → PC-side Lab Streaming Layer (LSL). Direct ESP32-to-LSL streaming is later development and is not included here. Earlier Pyboard and STM32 prototypes and the separate Chapter 6 EEG replay classifier are outside this collection.

The firmware association with the March 2022 study rests on preserved build dates and matching packet and recording content. No per-session flashing log survives. The author confirmed that the integrated board used for the March recordings was the design represented by the July revision 3 CAD; the exact March fabrication files and component population are not established by the July files.

## Use and citation

Start with the component READMEs. This is research source, not a turnkey acquisition system: the historical Python receiver is a separate dependency, local network settings must be supplied, and exact historical rebuilds have limitations. No participant recordings or original credential-bearing binaries are included.

Please cite the thesis by its title and author above and identify this collection by its archive SHA-256 and the component snapshot dates. No DOI has been assigned to this collection. The intended thesis and artefact access page is [matthewbarras.com/phd](https://matthewbarras.com/phd); a publicly downloadable thesis or repository thesis DOI was not verified during preparation.

The related platform paper is [Barras et al., Sensors 2026, DOI 10.3390/s26041342](https://doi.org/10.3390/s26041342). The separate Chapter 6 artefact is [Edge EEG Workload Classification v1.0.0](https://doi.org/10.5281/zenodo.20452990); it concerns replayed, preprocessed EEG and is not the acquisition firmware in this collection.

## Licensing and limits

Existing copyright, attribution and licence notices are retained in the files and summarised in [LICENSING.md](LICENSING.md). Selecting a public-release licence is outside this private reviewer handoff.

The thesis demonstrations support functional operation and physiological plausibility. They do not establish clinical validation, comprehensive ambulatory validation or regulatory readiness. The design must not be treated as approved for clinical use or as a verified manufacturing package. See [REVIEW.md](REVIEW.md) for unresolved items and verification results.
