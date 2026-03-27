# ler n
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

# n1, n2
addi $s1, $zero, 1
addi $s2, $zero, 1

# n1 <= n
WHILE:
slt $t0, $s0, $s1
bne $t0, $zero, FIM_WHILE

# imprima n1
addi $v0, $zero, 1
add $a0, $zero, $s1
syscall

add $s3, $zero, $s1 # aux = n1
add $s1, $zero, $s2 # n1 = n2
add $s2, $s2, $s3   # n2 = n2 + aux

j WHILE

FIM_WHILE:

addi $v0, $zero, 10
syscall