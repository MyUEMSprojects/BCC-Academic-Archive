main:
    addi $v0, $zero, 5   # syscall para leitura de inteiro
    syscall
    move $a0, $v0        # n armazenado em $a0
    jal soma             # chama a função soma
    move $a0, $v0        # resultado armazenado em $v0 movido para $a0
    addi $v0, $zero, 1   # syscall para imprimir inteiro
    syscall
    addi $v0, $zero, 10  # syscall para terminar o programa
    syscall

soma:
    addi $sp, $sp, -12   # reserva espaço na pilha
    sw $ra, 4($sp)       # salva o endereço de retorno
    sw $a0, 0($sp)       # salva o valor de $a0 (n)

    # Caso base: se n == 0, retorna 0
    beq $a0, $zero, caso_base

    # Loop interno: acumula soma de 1 até n
    li $t0, 0            # inicializa acumulador interno
    move $t1, $a0        # cópia de n para controle do loop
loop:
    beq $t1, $zero, fim_loop # termina o loop quando t1 == 0
    add $t0, $t0, $t1    # soma o valor de t1 ao acumulador
    addi $t1, $t1, -1    # decrementa t1
    j loop               # repete o loop

fim_loop:
    move $v0, $t0        # armazena o resultado do loop em $v0

    # Chamada recursiva
    addi $a0, $a0, -1    # decrementa n
    jal soma             # chamada recursiva
    lw $a0, 0($sp)       # restaura o valor original de $a0 (n)
    add $v0, $v0, $a0    # soma o resultado da recursão com o acumulador

    # Restaura registradores
    lw $ra, 4($sp)       # restaura $ra
    addi $sp, $sp, 12    # libera espaço na pilha
    jr $ra               # retorna

caso_base:
    li $v0, 0            # retorna 0 para caso base
    lw $ra, 4($sp)       # restaura $ra
    addi $sp, $sp, 12    # libera espaço na pilha
    jr $ra               # retorna
