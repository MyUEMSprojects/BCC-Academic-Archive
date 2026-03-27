
# n
addi $v0, $zero, 10
add $s0, $zero, $v0

# i
addi $v0, $zero, 0
add $s1, $zero, $v0

FOR:
slt $t0, $s1, $s0
beq $t0, $zero, FIM_FOR

addi $v0, $zero, 1
add $a0, $zero, $s1
syscall

addi $s1, $s1, 1

j FOR

FIM_FOR:

addi $v0, $zero, 10
syscall