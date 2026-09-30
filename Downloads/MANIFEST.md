# Local research artefact review packages

Generated from `PhD-Artefacts/` by `bash scripts/package_artefacts.sh`. Prepared for restricted online reviewer access, separate from the thesis itself. No upload was performed.

| Archive | Bytes | SHA-256 |
| --- | ---: | --- |
| PhD-Artefacts-review.zip | 1,801,707 | `ec3d627da98f7e2cf5c0575cfa44c5b73678dcc19965b5df839177b97e6e32cf` |
| PhD-Firmware-study-review.zip | 1,047,450 | `f5f919e4ad6c940fb0e7aeb749c2fac46fc26764b1977177aac8ab7c6d925690` |
| PhD-Hardware-rev3-review.zip | 751,411 | `04c052933ee69ec7026e5b6088e01b75653a97506854c337071071b68f08b0fd` |

The firmware packages include the recovered ESP32-S2 study source associated with 21 March 2022 and STM32H7 study source associated with 30 March 2022. They do not substitute older salvage strata or later direct-LSL code. The hardware package contains July revision 3 editable KiCad files and the verified separately preserved July fabrication files. Original firmware binaries remain private.

Each ZIP includes per-file SHA256SUMS (paths relative to the extraction directory; the checksum file excludes itself). Archives use fixed timestamps and sorted entries. Run `sha256sum -c SHA256SUMS` beside the archives, or against the internal file from the directory containing the extracted PhD-Artefacts folder.

Excluded: participant recordings; custom Python host software pending its own release review; earlier firmware/hardware lineage; enclosure designs; third-party standalone libraries and models of unresolved redistribution status; credentials; private build outputs. Historical mixed packages are retained under `legacy-private-2026-09-30/` and MUST NOT be uploaded as part of this candidate. Only the three files listed above and this manifest/checksum file are current downloads. See the packaged REVIEW.md and LICENSING.md for reproduction notes and retained attribution.
