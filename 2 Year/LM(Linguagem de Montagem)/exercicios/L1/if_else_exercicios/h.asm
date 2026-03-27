
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

slt $t0, $s0, $zero
bne $t0, $zero, ELSE

addi $v0, $zero, 1
addi $a0, $zero, 0
syscall
j FIM_ELSE

ELSE:

addi $v0, $zero, 1
addi $a0, $zero, 1
syscall

FIM_ELSE:

addi $v0, $zero, 10
syscall
