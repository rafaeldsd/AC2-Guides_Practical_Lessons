    .equ SFR_BASE_HI, 0xBF88    # 16 MSbits of SFR area
    .equ TRISE, 0x6100          # TRISE address is 0xBF886100
    .equ PORTE, 0x6110          # PORTE address is 0xBF886110
    .equ LATE, 0x6120           # LATE address is 0xBF886120
    .equ TRISD, 0x60C0          # TRISD address is 0xBF8860C0
    .equ PORTD, 0x60D0          # PORTD address is 0xBF8860D0
    .equ LATD, 0x60E0           # LATD address is 0xBF8860E0
    .data
    .text
    .globl main 

main:
    lui $t0, SFR_BASE_HI        # $t1=0xBF880000
    
    # Configuration of TRISE register (bit0=0 means OUTPUT)
    lw $t1, TRISE($t0)          # READ (Mem_addr = 0xBF880000 + 0x6100)
    andi $t1, $t1, 0xFFFE       # MODIFY (bit0=0 (0 means OUTPUT))
    sw $t1, TRISE($t0)          # WRITE (Write TRISE register)

    # Configuration of TRISD register (bit8=1 means INPUT)
    lw $t1, TRISD($t0)          # READ (Mem_addr = 0xBF880000 + 0x60C0)
    ori $t1, $t1, 0x0100        # MODIFY (bit8=1 (1 means INPUT))
    sw $t1, TRISD($t0)          # WRITE (Write TRISD register)

# In infinite loop, read value from input port and write this value to output port (i.e., RE0 = RB0).
loop:                          
    
    lw $t1, PORTB($t0)          # READ (Mem_addr = 0xBF880000 + 0x6050)
    andi $t1, $t1, 0x0100       # $t1 = RB8
    lw	$t2, LATE($t0)          # READ (Mem_addr = 0xBF880000 + 0x6120)
    andi	$t2, $t2, 0xFFFE    #  $t2 = RE0
    or	$t2, $t2, $t1           # $t2 = RE0 = RB8
    not $t2, $t2                # $t2 = RE0 = !RB8
    sw	$t2, LATE($t0)          # WRITE (Write LATE register)
    j loop                      # Infinite loop
    li $v0, 1                   # return 0
    jr $ra                      # Exit program

