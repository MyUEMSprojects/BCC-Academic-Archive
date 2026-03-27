# ler a
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

#ler b
addi $v0, $zero, 5
syscall
add $s1, $zero, $v0

# i
add $s2, $zero, $s0

FOR:
slt $t0, $s2, $s1  # i < b
beq $t0, $zero, FIM_FOR # i <= b

addi $v0, $zero, 1
add $a0, $zero, $s2
syscall

addi $s2, $zero, 1
j FOR

FIM_FOR:

addi $v0, $zero, 10
syscall
