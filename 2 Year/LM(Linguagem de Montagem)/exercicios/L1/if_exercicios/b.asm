addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

addi $v0, $zero, 0
add $s1, $zero, $v0

# if( a > 0) --> 0 < a
slt $t0, $s1, $s0
beq $t0, $zero, FIM_IF

addi $v0, $zero, 0
add $s2, $zero, $v0

addi $v0, $zero, 1
add $a0, $zero, $s2
syscall

FIM_IF:

addi $v0, $zero, 10
syscall