# Leitura do número n
main:
    addi $v0, $zero, 5   # syscall para leitura de inteiro
    syscall
    move $a0, $v0        # n é armazenado em $a0
    jal fatorial         # chama a função fatorial
    move $a0, $v0        # resultado em $v0, movido para $a0 para saída
    addi $v0, $zero, 1   # syscall para imprimir inteiro
    syscall
    addi $v0, $zero, 10  # syscall para encerrar
    syscall

# Função fatorial: calcula fatorial(n) recursivamente
fatorial:
    addi $sp, $sp, -12   # reserva espaço na pilha para $fp, $ra e $a0
    sw $fp, 8($sp)       # salva o antigo $fp
    sw $ra, 4($sp)       # salva o endereço de retorno
    sw $a0, 0($sp)       # salva o valor original de $a0 (n)
    move $fp, $sp        # atualiza $fp para a nova base do quadro

    # Caso base: se n <= 1, retorna 1
    ble $a0, 1, caso_base

    # Passo recursivo: fatorial(n) = n * fatorial(n - 1)
    addi $a0, $a0, -1    # decrementa n
    jal fatorial         # chamada recursiva para fatorial(n-1)

    # Restaura o valor original de $a0 (n) para a multiplicação
    lw $a0, 0($fp)
    mul $v0, $a0, $v0    # multiplica n * fatorial(n-1)

    # Restaura o estado anterior e libera a pilha
    lw $ra, 4($fp)       # recupera o valor antigo de $ra
    lw $fp, 8($fp)       # recupera o valor antigo de $fp
    addi $sp, $sp, 12    # libera o espaço na pilha
    jr $ra               # retorna para a chamada anterior

caso_base:
    li $v0, 1            # retorna 1 para caso base
    lw $ra, 4($fp)       # restaura $ra
    lw $fp, 8($fp)       # restaura $fp
    addi $sp, $sp, 12    # libera o espaço da pilha
    jr $ra               # retorna para a chamada anterior
