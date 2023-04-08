    .equ	SFR_BASE_HI, 0xBF88
	.equ	TRISE, 0x6100
	.equ	LATE, 0x6120
    # (...)
	.equ 	resetCoreTimer, 12
	.equ	readCoreTimer, 11
	.data
	.text
	.globl main
    
    # (...)

delay:

for:    ble     $a0,$0,endfor
        li      $v0, resetCoreTimer
        syscall

while: 
        li      $v0, readCoreTimer
        syscall
        blt     $v0,20000, while
        
        addi    $a0,$a0, -1
        j for
endfor:
        jr      $ra