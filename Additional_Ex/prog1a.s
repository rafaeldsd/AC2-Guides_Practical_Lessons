# Write a program in assembly language that, in an infinite loop, reads the 4 bits of the dip-switch of the DETPIC32 board (connected to ports RB3 to RB0)
# and show the value of each one of them on the 4 LEDs connected to ports RE3 to RE0.
    .equ SFR_BASE_HI, 0xBF88    # 16 MSbits of SFR area
    .equ TRISE, 0x6100          # TRISE address is 0xBF886100
    .equ PORTE, 0x6110          # PORTE address is 0xBF886110
    .equ LATB, 0x60A0           # LATB address is 0xBF8860A0
    .equ TRISB, 0x60A4          # TRISB address is 0xBF8860A4
    .data
    .text
    .globl main
main:       
    lui $t0, SFR_BASE_HI            # $t0 = 0xBF88

    # Configure RB3 to RB0 as inputs
    lw      $t1, TRISB($t0)         # $t1 = TRISB 
    ori     $t1, $t1, 0x000F        # $t1 = $t1 | 0x000F
    sw      $t1, TRISB($t0)         # TRISB = $t1

    # Configure RE3 to RE0 as outputs
    lw      $t1, TRISE($t0)         # $t1 = TRISE 
    andi    $t1, $t1, 0xFFF0        # $t1 = $t1 & 0xFFF0
    sw      $t1, TRISB($t0)         # TRISB = $t1
while:
    # Read the value of the dip-switch
    lw      $t1, PORTB($t0)         # $t1 = PORTB
    addi    $t1, $t1, 0x000F        # $t1 = $t1 + 0x000F
    # Write the value of the dip-switch to the LEDs
    lw      $t2, LATE($t0)          # $t2 = LATE
    andi    $t2, $t2, 0xFFF0        # $t2 = $t2 & 0xFFF0
    or      $t2, $t2, $t1           # $t2 = $t2 | $t1
    sw      $t2, LATE($t0)          # LATE = $t2
    j while

    li $v0, 0                       # return 0

    jr $ra                          # end of program