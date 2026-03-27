addi $v0, $zero, 5
syscall
add $s0, $zero, $v0  # Lê o valor de a e armazena em $s0

addi $v0, $zero, 5
syscall
add $s1, $zero, $v0  # Lê o valor de b e armazena em $s1

INICIO_DO_WHILE:
sub $t1, $s0, $s1
add $s2, $zero, $t1  # Calcula a diferença $s0 - $s1 e armazena em $s2

addi $v0, $zero, 1
add  $a0, $zero, $s2
syscall  # Imprime a diferença

addi $s0, $s0, -1  # Decrementa $s0

slt $t0, $s0, $s1  # Coloca 1 em $t0 se $s0 < $s1
beq $t0, $zero, INICIO_DO_WHILE  # Continua o laço enquanto $s0 >= $s1

addi $v0, $zero, 10
syscall  # Termina o programa
