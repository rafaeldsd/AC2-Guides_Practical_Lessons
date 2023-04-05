# Write a program in assembly that runs in an infinite loop, and that whenever the user presses a key on the PC, does the following tasks:
# - key 0: should light only the LED connected to port RE0;
# - key 1: should light only the LED connected to port RE1;
# - Key 2: should light up only the LED connected to port RE2
# - key 3: only the LED connected to port RE3 should light up;
# - any other key: the 4 LEDs should light up and remain on for 1 second; at the end of this time the 4 LEDs should be turned off (this must be the initial state of the 4 LEDs).
# Any key pressed within that time should be ignored.
# Use the PIC32 core timer to control the timing.

    .equ SFR_BASE_HI, 0xBF88    # 16 MSbits of SFR area
    .equ TRISE, 0x6100          # TRISE address is 0xBF886100
    .equ PORTE, 0x6110          # PORTE address is 0xBF886110
    .equ LATB, 0x60A0           # LATB address is 0xBF8860A0
    .equ TRISB, 0x60A4          # TRISB address is 0xBF8860A4
    .equ READ
    .data
    .text
    .globl main
main:       