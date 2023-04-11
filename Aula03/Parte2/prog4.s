    .equ RESET_CORE_TIMER, 12
    .equ READ_CORE_TIMER, 11
    .equ SFR_BASE_HI, 0xBF88    # 16 MSbits of SFR area
    .equ TRISE, 0x6100          # TRISE address is 0xBF886100
    .equ PORTE, 0x6110          # PORTE address is 0xBF886110
    .equ LATE, 0x6120           # LATE address is 0xBF886120
    .equ TRISB, 0x6040          # TRISB address is 0xBF886040
    .equ PORTB, 0x6050          # PORTB address is 0xBF886050
    .data
    .text
    .globl main 

main:
    addiu $sp, $sp, -12       # Reserve 12 bytes on stack
    sw $ra, 0($sp)            # Save return address
    sw $s0, 4($sp)            # Save $s0
    sw $s1, 8($sp)            # Save $s1

    # CONFIGURATION OF PORTS
    lui    $t0, SFR_BASE_HI    # Load SFR_BASE_HI into $t0

    # Configuration of TRISE register (bit4=0,bit3=0,bit2=0,bit1=0 means OUTPUT)
    lw $t1, TRISE($t0)          # READ (Mem_addr = 0xBF880000 + 0x6100)
    andi $t1, $t1, 0xFFE1       # Set bit4,bit3,bit2,bit1 as output
    sw $t1, TRISE($t0)          # WRITE (Mem_addr = 0xBF880000 + 0x6100)

    lw $t1, TRISB($t0)          # READ (Mem_addr = 0xBF880000 + 0x6040)
    ori $t1, $t1, 0x0002        # Set bit1 as input
    sw $t1, TRISB($t0)          # WRITE (Mem_addr = 0xBF880000 + 0x6040)

    li $s1, 0                  # Initialize $s1 to 0

loop:
    lw $t1, LATE($t0)           # READ (Mem_addr = 0xBF880000 + 0x6120)
    andi $t1, $t1, 0xFFE1       # Clear bit4,bit3,bit2,bit1
    or $t1, $t1, $s1            # OR with $s0
    sw $t1, LATE($t0)           # WRITE (Mem_addr = 0xBF880000 + 0x6120)

    li $a0, 333
    jal delay

    lw $t2, PORTB($t0)         # READ (Mem_addr = 0xBF880000 + 0x6000)
    andi $t2, $t2, 0x0002      # read bit3

if: beq $t2, $0, else
    sll $s1, $s1, 1            # Shift $s1 left by 1
    j endif

else:
    srl $s1, $s1, 1           # Shift $s1 right by 1
    
endif: 
    andi $s1, $s1, 0x001E      # Clear bit4,bit3,bit2,bit1
    j loop

    # RESTORE REGISTERS

    lw $ra, 0($sp)            # Restore return address
    lw $s0, 4($sp)            # Restore $s0
    lw $s1, 8($sp)            # Restore $s1
    addiu $sp, $sp, 12        # Restore stack pointer
    jr $ra                    # Return to caller

delay:
for:
    ble $a0,$0,endfor
    li $v0, RESET_CORE_TIMER
while:
    li $v0, READ_CORE_TIMER
    syscall
    blt $v0,20000,while
    addi $a0, $a0, -1
    j for
endfor:
    jr $ra
    



