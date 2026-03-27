addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

addi $v0, $zero, 5
syscall
add $s1, $zero, $v0

addi $v0, $zero, 5
syscall
add $s2, $zero, $v0

addi $v0, $zero, 5
syscall
add $s3, $zero, $v0

#compara��o e troca
#a com b
slt $t0, $s0, $s1
bne $t0, $zero, FIM_IF_1
add $t0, $zero, $s0
add $s0, $zero, $s1
add $s1, $zero, $t0
FIM_IF_1
#imprima a
addi $v0, $zero, 1
