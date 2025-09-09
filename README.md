# bare-metal-breakout
A classic breakout game written in C and assembly to run in logisim or QEMU RISC-V simulators.

bare-metal-breakout running in logisim on a virtual RISCV CPU: \
<img width="500" height="480" alt="image" src="https://github.com/user-attachments/assets/ec744cee-2780-45cf-ac60-e023a2118909" />

Running in a custom QEMU emulator: \
<img width="599" height="259" alt="image" src="https://github.com/user-attachments/assets/3bcc7159-0a3e-4fe3-9ff6-82a27755e667" />



## Overview
This is a bare-metal Breakout game implemented in C and RISC-V assembly, developed as part of a computer architecture course. It builds on the “c-and-assembly” demo project from Pavel Gladyshev’s [COMP20180-logisim-riscv](https://github.com/pavelgladyshev/COMP20180-logisim-riscv/tree/main) repository, originally designed for a Logisim RISC-V computer and QEMU simulation. The project demonstrates startup assembly, linker scripts, memory-mapped I/O for graphics and input, combined with game logic in C using bit-level operations.

## Files
breakout.c       - Main program
game_logic.c/h   - Game logic
lib.c/h          - Low-level I/O library (CC-BY 4.0)
crt0.S           - Startup code
showpic.s        - Graphics routine
logisim.lds      - Linker script for Logisim
qemu.lds         - Linker script for QEMU comp20180
Makefile         - Build rules

## Build
Requires riscv64-unknown-elf toolchain.
make            : build Logisim ROM (breakout-rom.txt)
make qemu       : run on QEMU comp20180
make clean      : remove artifacts

## Usage Notes
The game is tied to Logisim’s MMIO and the comp20180 machine model from coursework, so it is not directly portable to native or standard hardware. The repository includes source, build files, and visuals so that anyone reviewing the code can understand both framework and functionality.

## License
The files lib.c and lib.h are licensed under Creative Commons Attribution 4.0 International (CC BY 4.0) by their original author.
