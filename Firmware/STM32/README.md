# STM32H743 study acquisition firmware

Target: STM32H743VIT6 (Cube project target `STM32H743VITx`), Cortex-M7. The firmware services four ADS1299 converters, sequences the TLC5940 optical driver and exchanges configuration/data with the ESP32-S2. It is the Chapter 4 acquisition snapshot associated with the **30 March 2022, 15:43:28 BST** Release build, not the earlier 11 March development stratum.

The preserved binary has SHA-256 `4acc7a23546c86fc647d239a78a6179f4e8ad333f247e25bd412178ced164ba8`. Original binary, ELF and listing remain private provenance. No per-session flashing log survives. See [provenance](../../PROVENANCE.md).

## Contents and dependencies

- `Core/Src`: application, ADS1299 and TLC5940 routines, interrupts and system support.
- `Core/Inc`: application and HAL configuration headers.
- `Core/Startup`: STM32H743 startup assembly.
- `Drivers`: preserved STM32 HAL and CMSIS dependency files, with existing licence notices.
- `.project`, `.cproject`: CubeIDE project and build configurations.
- `iobio2.ioc`: CubeMX configuration, retained as provenance. Regeneration may overwrite study code.
- `STM32H743VITX_FLASH.ld`, `STM32H743VITX_RAM.ld`: linker scripts.
- `ReleaseRecipe`: preserved generated Release makefiles and link inputs, with the machine-specific linker path made relative. These are included to reproduce build settings, not as compiled outputs.

The `.ioc` records CubeMX **6.2.0** and **STM32Cube FW_H7 V1.9.1**. The Release makefile and ELF compiler string identify **GNU Tools for STM32 9-2020-q2-update**, GCC **9.3.1 20200408**. The `.cproject` still names the older 7-2018-q2-update toolchain; do not treat that stale IDE field as the compiler used for the archived Release. The exact CubeIDE application version is unverified.

## Build and flashing

With the historical GNU Arm compiler and GNU Make on `PATH`, from this directory:

```sh
make -C ReleaseRecipe all
```

Specify `all` explicitly: the generated makefile's included rules can otherwise select a clean target. The recipe preserves Cortex-M7, hard-float FPv5-D16, GNU11, `-O0`, section garbage collection, nano/nosys specifications and the original flash linker script. Expected outputs are `ReleaseRecipe/iobio2.elf`, `.bin`, `.map` and `.list`.

Alternatively import `.project` into CubeIDE and select Release, checking the compiler against the build provenance above. Do not regenerate from the `.ioc` as part of reproducing this source.

Release preparation compiled every historical C and startup-assembly input using available Arm GNU **11.3.1**. Linking with that compiler failed on duplicate `ADS_Buffer` definitions. Existing warnings include a non-void function without a return and an unresolved USB transmit declaration in discarded code. These were not repaired. A full historical-toolchain rebuild and binary equivalence remain unverified; newer compiler behaviour must not be silently patched into the study snapshot.

The flash linker script places the application at `0x08000000`. A programming session would load the rebuilt ELF via a compatible STM32 programming/debugging setup and verify the programmed data. The exact historical probe, connection and upload sequence are not established, so no supposedly verified flashing command is provided. No board was flashed during release preparation.

## Signals and data flow

Pin definitions come from `Core/Inc/main.h` and `Core/Src/stm32h7xx_hal_msp.c`.

| Function | STM32 pins |
| --- | --- |
| ADS SPI1 SCK/MISO/MOSI | PA5 / PA6 / PA7 |
| ADS1/2/3/4 chip selects | PA4 / PE12 / PE11 / PE10 |
| Shared start, reset, power-down | PC5 / PB0 / PB1 |
| ADS1 data-ready interrupt | PC4, EXTI4 |
| ESP32 SPI3 NSS/SCK/MISO/MOSI | PA15 / PC10 / PC11 / PC12 |
| ESP32 command-ready interrupt | PD8 |
| TLC SPI2 SCK/MOSI/MISO | PB10 / PC1 / PC2_C |
| TLC blank / grayscale clock / programming / latch / error | PC6 / PC7 / PC8 / PC9 / PA10 |
| Blue / green / red status indicators | PE2 / PE3 / PE4 |
| Historical ADS error-indicator routine | PE1 |
| Configured USART2 TX/RX | PA2 / PA3 |

ADS1 data-ready schedules foreground work. Five phases activate the optical pattern, tag phase 2 and blank at phase 3; twelve patterns are cycled. Descending 27-byte converter reads overwrite the intermediate status words, leaving one three-byte state and all 32 channel words in each 99-byte record. Every twentieth record is sent in a 2048-byte SPI packet. The ESP32 forwards it to the custom Python receiver for PC-side LSL publication.

Host configuration is supplied through that receiver and relayed over the ESP32. Preserve the historical parser and register writes when reproducing the study; no new configuration interface is added. The host's channel and LED masks do not change the fixed acquisition schedule in this snapshot. Source-requested settings are not evidence of a retained per-session register readback.

## Limitations and publication edits

This snapshot does not recover the complete storage-enabled implementation. Unused variables, historical register logic, retry-state behaviour and active instrumentation remain intact. There is no new validation of electrical safety, worst-case interrupt/service latency or loss-free streaming.

Two unprofessional identifiers were renamed consistently to `IndicateADSError` and `adsInitAttemptState`, including declarations and call sites. Their logic is unchanged. Comments explain the optical slot, overlapping record packing, error indication and the HAL transfer timeout. Obsolete commented-out TLC helper routines were removed. A relative linker path replaces the original workstation path. No algorithm, pin, delay, register value or packet field was corrected. Authorship and vendor notices are retained; see [licensing](../../LICENSING.md).
