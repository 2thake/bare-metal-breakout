/*
 * Example software running in Logisim RISC-V Computer System model by Pavel Gladyshev
 * licensed under Creative Commons Attribution International license 4.0
 *
 * Some minor changes made by John Hughes, namely adding constant definitions
 *
 *
 * This is a header file that declares utility functions for reading and printing strings and numbers
 * from/to MMIO Display and Keyboard
 *
 * See lib.c for explanations
 */

#pragma once

#ifdef QEMU20180

#define RDR ((volatile char *)0x10000000)
#define RCR ((volatile char *)0x10000005)
#define RDR_READY_BIT (1)
#define TDR ((volatile char *)0x10000000)
#define TCR ((volatile char *)0x10000005)
#define TDR_READY_BIT (1 << 5)
#define ENTER_CHAR_CODE (13)
#define START_MSG "Press any key on the MMIO keyboard to start\nControl keys: a - move paddle left; d - move paddle right\n"
#define SLOWDOWN 10000

#else // Logisim RV32IM model

#define RDR ((volatile char *)0xFFFF0004)
#define RCR ((volatile char *)0xFFFF0000)
#define RDR_READY_BIT (1)
#define TDR ((volatile char *)0xFFFF000C)
#define TCR ((volatile char *)0xFFFF0008)
#define TDR_READY_BIT (1)
#define ENTER_CHAR_CODE (10)
#define START_MSG "Press START button\nUse the slider to move the paddle left or right\n"
#define SLOWDOWN 0

#endif

// Math
int abs(int n);

// MMIO Display output
void printchar(char chr);
void printstr(char *str);
void printint(int n);
void println();

// MMIO Keyboard input
int pollkbd();
int readchar();
int readstr(char *buf, int size);
int readint();

// Monochrome bitmap display output
void showpic(int picture[32]);
void display_score(int score);
