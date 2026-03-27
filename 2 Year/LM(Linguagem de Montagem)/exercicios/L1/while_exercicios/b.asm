addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

addi $v0, $zero, 5
syscall
add $s1, $zero, $v0

INICIO_WHILE:
slt $t0, $s1, $s0
beq $t0, $zero, FIM_WHILE

addi $v0, $zero, 1
add  $a0, $zero, $s0
syscall

addi $s0, $s0, -1
j INICIO_WHILE

FIM_WHILE:

addi $v0, $zero, 10
syscall