# Snapshot identifiers and change ledger

Prepared 30 September 2026 from the March 2022 study firmware and July 2022 revision 3 hardware. The firmware build hashes below are the identifiers listed in thesis Appendix B. Originals remain unchanged in the author's evidence archive.

| Preserved original | SHA-256 |
| --- | --- |
| STM32 Release ELF, 30 March 2022 | `5cad2942acf60bbd2424e283123c9c237ac0ab2aa6d822ed147228782f812b76` |
| STM32 Release BIN | `4acc7a23546c86fc647d239a78a6179f4e8ad333f247e25bd412178ced164ba8` |
| STM32 Release listing | `7356f2bef4a930e69513019848cc4b48311863e5264c303b8e15d3896c440104` |
| ESP32-S2 ELF, 21 March 2022 | `3ba8441698d402facbca2961f0747c7e87f7935585ec80bc78d4e89df5f08394` |
| ESP32-S2 BIN | `a46ada9cb61cc105341fa3b6d3193f429d0d66189247051dc5a8554155196353` |
| Revision 3 hardware ZIP | `58faf109a45be48c7d09134c4aff16fe88dc38e7e3fb09e51d6ca540b09b4f85` |
| Selected latest PCB, 15 July 2022 15:24:40 | `4037a89526a83346fe1681b8de66f691b875b99b842814eeb6874a4de77ba51e` |
| Earlier PCB inside the source ZIP, 15 July 2022 14:37:12 | `4d30cfdb4a4ace5f22cbb350cfa149e7041f613be833e6bd94590446447efd3c` |
| Separate 15 July 2022 fabrication ZIP | `64adf6c37c6a710a4e4d792d957dc0484377da9a16fb40b9c8a8860508a579c0` |

The hardware archive matches Git blob `4a71b1dacb59477e811960788c41c4c6abbc14a1` at commit `d50d4b3d22bedd5c237dd0a5a69f158319e18c09`, verified by SHA-256.

The selected 15:24 PCB contains same-day silkscreen updates and additional precision in four rounded-pad ratios compared with the 14:37 archive copy. Its project and seven schematic sheets match the archive after line-ending normalisation. Preparation checks use the selected 15:24 original as the baseline.

Appendix B records the study-build association. Earlier firmware, later direct-LSL development and the edge-replay release are separate artefacts.

## Changes from originals

| Category | Changes |
| --- | --- |
| Whitespace/formatting | Existing line-ending styles retained; local spacing/newlines change around rewritten comments. No whole-project formatter applied. |
| Comment/documentation | ESP32 forwarding/handshake comments corrected; unused commented alternative implementations removed. STM32 record packing, optical phase, retry state and HAL timeout comments clarified. New READMEs, provenance, licensing and review notes added. |
| Identifier presentation | Two STM32 identifiers renamed consistently to `IndicateADSError` and `adsInitAttemptState`, retaining their call sites, operations and constants. |
| Credential/personal-data redaction | ESP32 AP and station credentials replaced by explicit local-configuration placeholders; unused private-IP macro removed; credential-printing log call removed. The STM32 makefile's absolute linker path becomes relative. Two optional PCB 3D-model references to private workstation paths removed. |
| File exclusion | Original firmware binaries, build caches, logs, workspace metadata and launch preferences omitted; build inputs and vendor source retained. KiCad autosaves, caches, personal settings, legacy inactive sheets, old PDFs, old Gerbers, redundant archives and unverified standalone downloaded libraries/models omitted. No original was deleted. |
| Executable behaviour | Acquisition, protocol, pin, register, scheduler and packet logic retained. Network settings require local configuration. Removing the credential log changes startup diagnostic output and execution time. |
| Circuit/PCB | No electrical or layer-geometry change. Two optional 3D references removed; a verified-dead download URL removed from footprint descriptions while retaining the IPC document/page attribution. |

Every copied file is compared with its original. C/H token checks allow the documented changes above. All STM32 link-input sources and ESP32 CMake application inputs are retained. These checks establish source-level preservation; builds are summarised in [REVIEW.md](REVIEW.md).

`FILE-PROVENANCE.csv` in the complete collection gives per-file original and prepared-file hashes. Download archives include per-file `SHA256SUMS`; the adjacent manifest lists archive checksums. Original binaries and detailed preparation logs are retained separately.
