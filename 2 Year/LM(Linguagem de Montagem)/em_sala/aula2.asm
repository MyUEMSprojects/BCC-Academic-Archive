add $s0,$s1, $2
# b = c + 10
addi $s1, $s2, 10 # adicionar  a um registrador um valor
# sub --> subtração de registradores, para i usa addi -valor
addi $s1, $2, -10

# a = a + b - c
add $t0,$s0, $1 # t0 recebe a + b
sub $s0, $t0, $2 # a recebe t0 - c

# a = b * c
# mul nao detecta overflow
mul $s0, $s1, $2
# ou
mult $s1, $s2 # gera um long int
mflo $s0

#div calcula a divisao e o resto da divisao
div $s2, $s3
mfhi $s0 # pega o resto da divisao

# exercicio:
# leia a,b,c
# imprima [(a + b) / (c%a)] * 2

addi $