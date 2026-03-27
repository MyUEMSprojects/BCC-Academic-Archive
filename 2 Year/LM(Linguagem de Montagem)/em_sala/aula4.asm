# slt
# a < b
# a > b => b < a
# a <= b => !(b < a)
# a >= b => !(a < b)

# slti
# p/ inteiros (i-valor inteiro imediato)
# a < i
# a > i => a >= i + 1 => !(a<i+1)
# a <= i => a < i + 1
# a >= i => !(a<i)

# if(<exp-logica>) {
#   <corpo>   
#}
#inst-apos-if
#
# MIPS
# - obter o valor lógico (F=0 e V!=0)
# - comparação
# - branch(bne ou beq)
#   - saltar p/ o endereço(label) da instrução após o if
# - corpo
# - inst-apos-if

##########

# if( a < b) {
#  imprima a
#}
# a -> $s0 e b -> $s1
slt $t0, $s0, $s1
# $t0 = 0 se !(a<b) e $t0 = 1 se a < b
beq  $t0, $zero, FIM_IF
addi $v0, $zero, 1
add $a0, $zero, $s0 #impressão
syscall
FIM_IF:
##########
# if (a >= b) { // !(a<b)
# imprima a
#}

slt $t0, $s0, $s1
# $t0 = 0 se !(a <b) e $t0 = 1 se a < b
bne $t0, $zero, FIM_IF
addi $v0, $zero, 1
add $a0, $zero, $s0 #impressão
syscall
FIM_IF:

# Operadores lógicos
# if(a==b)
# beq $s0, $1, FIM_IF
#if (a!=b)
#bne $s0, $s1, FIM_IF

# c = (a==b)
# c = !(a-b);

############
    
# if (a < b $$ b < c) {
#  corpo
#}
# a -> $s0 b -> $s1 c -> $s2
slt $t0, $s0, $s1
slt $t1, $s1, $s2
and $t2, $t0, $t1 # $t0 && $t1
# and analisa valores booleanos
# if (a && b) { // a!=0 && b!=0    (a<0 || a>0)&&(b<0 || b>0) !(a==0 || b==0)
#  corpo
#}

#!(a==0 || b==0)
or $t0, $s0, $s1
# $t0 != 0
bne $t0, $zero, FIM_IF
#
#
FIM_IF:

#########################

# escreva um programa em MIPS que leia 4 valores
# imprima estes valores em ordem crescente, usando
# apenas 8 syscalls( 4 leitura e 4 impressão)

