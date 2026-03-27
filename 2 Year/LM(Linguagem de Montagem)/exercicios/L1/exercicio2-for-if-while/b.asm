addi $s0, $zero, 0
addi $s1, $zero, 2
addi $s2, $zero, 100

FOR:
slt $t0, $s2, $s1
bne $t0, $zero, FIM_FOR

add $s0, $s0, $s1

addi $t1, $zero, 2
mul  $s1, $s1, $t1
j FOR

FIM_FOR:

addi $v0, $zero, 1
add $a0, $zero, $s0
syscall

addi $v0, $zero, 10
syscall