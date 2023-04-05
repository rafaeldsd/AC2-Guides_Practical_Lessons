# Change the program you wrote in a), so that the value of each bit of the dipswitch is displayed in reverse order on the 4 LEDs (i.e. RB3->RE0, RB2->RE1, ..., RB0->RE3).
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
    andi    $t2, $t1, 1             # $t2 = RB0
    andi    $t3, $t1, 2             # $t3 = RB1
    andi    $t4, $t1, 4             # $t4 = RB2
    andi    $t5, $t1, 8             # $t5 = RB3
    # Write the value of the dip-switch to the LEDs
    lw      $t6, LATE($t0)          # $t6 = LATE
    andi    $t7, $t6, 1             # $t7 = RE0
    andi    $t8, $t6, 2             # $t8 = RE1
    andi    $t9, $t6, 4             # $t9 = RE2
    andi    $t10, $t6, 8            # $t10 = RE3
    
    or      $t2, $t7, $t5           # $t2 = RE0 | RB3
    or      $t3, $t8, $t4           # $t3 = RE1 | RB2
    or      $t4, $t9, $t3           # $t4 = RE2 | RB1
    or      $t5, $t10, $t2          # $t5 = RE3 | RB0

    sw      $t2, LATE($t0)          # LATE = $t2
    sw      $t3, LATE($t0)          # LATE = $t3
    sw      $t4, LATE($t0)          # LATE = $t4
    sw      $t5, LATE($t0)          # LATE = $t5

    j while

    li $v0, 0                       # return 0

    jr $ra                          # end of program