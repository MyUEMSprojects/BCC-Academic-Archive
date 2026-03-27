# ler a
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

addi $v0, $zero, 0
add $s1, $zero, $v0

# if(a < 0) imprima 0
slt $t1, $s0, $s1
beq $t1, $zero, ELSE

addi $v0, $zero, 0
add $s2, $zero, $v0

addi $v0, $zero, 1
add $a0, $zero, $s2
syscall

ELSE:

addi $v0, $zero, 10
syscall