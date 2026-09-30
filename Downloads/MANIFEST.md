# Online Appendix downloads

Generated from `PhD-Artefacts/` by `bash scripts/package_artefacts.sh`. Hardware and study firmware for private reviewer access.

| Archive | Bytes | SHA-256 |
| --- | ---: | --- |
| PhD-Artefacts-review.zip | 1,795,918 | `0207c31f3f8aeef2d1cb43b34644e9614c877030d92b8437edf7c75be427c363` |
| PhD-Firmware-study-review.zip | 1,043,484 | `d0d2c715fa2c46b348eee0832e2434777d7d8c215cd641aad24a237c4dac508e` |
| PhD-Hardware-rev3-review.zip | 748,106 | `3404a836aed5c760ee7cbe7db55525a76567ccdae0bd4732eac0c7453fdb280e` |

The firmware packages contain the ESP32-S2 source snapshot of 21 March 2022 and STM32H7 study source associated with 30 March 2022. The hardware package contains July revision 3 editable KiCad files and the verified July fabrication files.

Each ZIP includes per-file SHA256SUMS (paths relative to the extraction directory; the checksum file excludes itself). Archives use fixed timestamps and sorted entries. Run `sha256sum -c SHA256SUMS` beside the archives, or against the internal file from the directory containing the extracted PhD-Artefacts folder.

These downloads cover the main-board design and study streaming firmware. The host application, recordings, mechanical designs and earlier development versions are separate artefacts. Component READMEs give build and project-opening instructions; PROVENANCE.md records snapshot identifiers and preparation changes.
