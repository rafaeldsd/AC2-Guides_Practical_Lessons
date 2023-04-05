delay:
for:
    ble $a0,$0,endfor
    li $v0, RESET_CORE_TIMER
while:
    li $v0, READ_CORE_TIMER
    syscall
    blt $v0,2000,while
    addi $a0, $a0, -1
    j for
endfor:
    jr $ra