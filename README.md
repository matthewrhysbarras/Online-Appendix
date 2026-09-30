# Online Appendix

Hardware and embedded firmware accompanying Matthew Rhys Barras's University of Hull PhD thesis, *Toward Mobile Neuroimaging: Design and Implementation of a Real-Time Multi-Modal EEG/fNIRS System*.

## Contents

| Directory | Contents | Thesis reference |
| --- | --- | --- |
| [Hardware](Hardware/README.md) | Revision 3 KiCad main-board project and July 2022 fabrication files | Chapter 3; Appendix A |
| [Firmware/STM32](Firmware/STM32/README.md) | STM32H743 acquisition firmware, 30 March 2022 snapshot | Chapter 4; Appendix B |
| [Firmware/ESP32](Firmware/ESP32/README.md) | ESP32-S2 communications firmware, 21 March 2022 snapshot | Chapter 4; Appendix B |

The study data path is STM32 acquisition → ESP32-S2 TCP forwarding → Python receiver → PC-side Lab Streaming Layer (LSL). The firmware retains the study packet format and optical schedule described in Chapter 4.

This repository covers the main-board design and study streaming firmware. The host application, recordings, mechanical designs and earlier development versions listed in the thesis are separate artefacts. Chapter 6's replay experiment is available as [Edge EEG Workload Classification v1.0.0](https://doi.org/10.5281/zenodo.20452990).

## Using the files

Start with the component READMEs for project layout, toolchains, configuration and build instructions. The hardware README explains how to open the KiCad project. These are research-prototype files; fabrication and participant use require appropriate engineering and safety review.

[PROVENANCE.md](PROVENANCE.md) records snapshot identifiers and preparation changes. [REVIEW.md](REVIEW.md) summarises verification. Existing attribution and licence notices are retained; see [LICENSING.md](LICENSING.md).

For citation, use the thesis title and author above, together with the relevant snapshot date or archive checksum. The thesis artefact access route is [matthewbarras.com/phd](https://matthewbarras.com/phd).
