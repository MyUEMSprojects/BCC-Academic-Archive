# if(a >= 0){
#
#  imprima 0;
#}

addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

slt $t0, $s0, $zero # $t0 = 1 se $s0 < 0, $t0 = 0 se $s0 >= 0
bne $t0, $zero, FIM_IF # Se $t0 != 0, ou seja, se $s0 < 0, pula para FIM_IF

addi $v0, $zero, 1
addi $a0, $zero, 0
syscall 

FIM_IF:

addi $v0, $zero, 10
syscall