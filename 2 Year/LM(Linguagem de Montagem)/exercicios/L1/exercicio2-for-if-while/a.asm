addi $s0, $zero, 0
addi $s1, $zero, 20

addi $s2, $zero, 2

# for(i=0;i<=20;++i)

FOR:
slt $t0, $s1, $s0
bne $t0, $zero, FIM_FOR

# if(i%2 == 0)
#   imprime i

    div $s0, $s2
    mfhi $t1 # resto da divisão
    bne $t1, $zero, FIM_IF

    addi $v0, $zero, 1
    add $a0, $zero, $s0
    syscall

    FIM_IF:

addi $s0, $s0, 1
j FOR

FIM_FOR:

addi $v0, $zero, 10
syscall