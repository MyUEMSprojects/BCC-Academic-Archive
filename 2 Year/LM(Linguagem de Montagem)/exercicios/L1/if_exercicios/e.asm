#ler a
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

#ler b
addi $v0, $zero, 5
syscall
add $s1, $zero, $v0

#ler c
addi $v0, $zero, 5
syscall
add $s2, $zero, $v0

slt $t0, $s1, $s0
slt $t1, $s0, $s2
or  $t2, $t0, $t1
beq $t2, $zero, FIM_IF

addi $v0, $zero, 0
add $s3, $zero, $v0

addi $v0, $zero, 1
add $a0, $zero, $s3
syscall

FIM_IF:

addi $v0, $zero, 10
syscall