# STM32H743 acquisition firmware

The STM32H743 project reads four ADS1299 converters, sequences the TLC5940 optical driver and transfers acquisition packets to the ESP32-S2. It implements the study acquisition path described in Chapter 4.

Target: STM32H743VIT6, Cortex-M7 (project target `STM32H743VITx`). Snapshot: 30 March 2022, 15:43:28 BST Release build, identified in Appendix B. Original build hashes are listed in [PROVENANCE.md](../../PROVENANCE.md).

## Project layout

- `Core/Src`, `Core/Inc`: application, ADS1299 and TLC5940 routines, interrupts and headers.
- `Core/Startup`: startup assembly.
- `Drivers`: STM32 HAL and Arm CMSIS dependencies.
- `.project`, `.cproject`: CubeIDE project.
- `iobio2.ioc`: CubeMX configuration.
- `STM32H743VITX_FLASH.ld`, `STM32H743VITX_RAM.ld`: linker scripts.
- `ReleaseRecipe`: Release makefiles and link inputs.

## Toolchain and build

The Release metadata identifies GNU Tools for STM32 9-2020-q2-update, GCC 9.3.1. The CubeMX configuration records version 6.2.0 and STM32Cube FW_H7 V1.9.1. The exact CubeIDE version is unrecorded; its project file retains an older compiler selection.

With the historical compiler and GNU Make on `PATH`, run:

```sh
make -C ReleaseRecipe all
```

Specify `all` explicitly. The recipe retains Cortex-M7, hard-float FPv5-D16, GNU11, `-O0` and the flash linker script. Outputs are `ReleaseRecipe/iobio2.elf`, `.bin`, `.map` and `.list`.

Alternatively, import `.project` into CubeIDE, select Release and check the compiler selection. Regenerating from `iobio2.ioc` can overwrite application code.

All C and startup-assembly inputs compiled with GNU Arm 11.3.1 during preparation; linking reported duplicate `ADS_Buffer` definitions. An exact GCC 9.3.1 rebuild has not been verified.

## Programming

The flash linker script places the application at `0x08000000`. Program a successfully built ELF using a compatible STM32 programmer/debugger and verify the programmed data. The historical probe and connection procedure are not documented here; no device flashing was performed during preparation.

## Signals and operation

| Function | STM32 pins |
| --- | --- |
| ADS SPI1 SCK/MISO/MOSI | PA5 / PA6 / PA7 |
| ADS1/2/3/4 chip selects | PA4 / PE12 / PE11 / PE10 |
| Shared start / reset / power-down | PC5 / PB0 / PB1 |
| ADS1 data-ready interrupt | PC4, EXTI4 |
| ESP32 SPI3 NSS/SCK/MISO/MOSI | PA15 / PC10 / PC11 / PC12 |
| ESP32 command-ready interrupt | PD8 |
| TLC SPI2 SCK/MOSI/MISO | PB10 / PC1 / PC2_C |
| TLC blank / grayscale clock / programming / latch / error | PC6 / PC7 / PC8 / PC9 / PA10 |
| Blue / green / red indicators | PE2 / PE3 / PE4 |
| ADS error indicator | PE1 |
| USART2 TX/RX | PA2 / PA3 |

ADS1 data-ready schedules foreground acquisition. Each five-period optical slot activates the source, tags phase 2 and blanks at phase 3. Twelve conditions are cycled. Each 99-byte record carries the state field and all 32 channel values; twenty records form a 2048-byte packet. The ESP32 forwards packets to the Python receiver for PC-side LSL publication.

The host supplies mode, gain and rate settings through the ESP32. Channel-enable and LED-enable masks do not alter this snapshot's fixed acquisition schedule. This is the streaming implementation; SD logging is outside this source snapshot.

## Preparation changes

Two identifiers and explanatory comments were clarified, obsolete commented-out helpers removed and the linker path made relative. Acquisition logic, register values, pins, delays and packet fields are unchanged. See [PROVENANCE.md](../../PROVENANCE.md) for the change ledger. Contributor and vendor notices remain.
