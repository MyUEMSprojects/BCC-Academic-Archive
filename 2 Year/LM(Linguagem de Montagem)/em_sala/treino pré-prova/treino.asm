.data
array: .space 100   # array de tamanho máximo 100
n: .word 0           # variável para armazenar o tamanho do array
newline: .asciiz "\n"
space: .asciiz " "
quote: .asciiz "\""

.text
main:
    # Leitura do tamanho do array
    addi $v0, $zero, 5
    syscall
    add $t0, $zero, $v0       # $t0 armazena o tamanho do array

    # Loop para ler os elementos do array
    la $t1, array       # $t1 aponta para o início do array
    addi $t2, $zero, 0
read_loop:
    bge $t2, $t0, print_array  # se o contador >= tamanho do array, sair do loop
    addi $v0, $zero, 5
    syscall
    sw $v0, ($t1)       # armazena o elemento lido no array
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
    j read_loop

print_array:
    la $t1, array       # $t1 aponta para o início do array
    addi $t2, $zero, 0

    # Encontra o maior e o menor elemento
    lw $t3, ($t1)       # carrega o primeiro elemento do array
    add $t4, $zero, $t3 # $t4 armazena o maior elemento
    add $t5, $zero, $t3 # $t5 armazena o menor elemento
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
find_max_min_loop:
    bge $t2, $t0, print_with_quotes  # se o contador >= tamanho do array, sair do loop
    lw $t6, ($t1)       # carrega o próximo elemento do array
    ble $t6, $t5, update_min    # se o elemento <= menor elemento, atualiza o menor elemento
    bge $t6, $t4, update_max    # se o elemento >= maior elemento, atualiza o maior elemento
    j continue_loop
update_min:
    add $t5, $zero, $t6
    j continue_loop
update_max:
    add $t4, $zero, $t6
continue_loop:
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
    j find_max_min_loop

print_with_quotes:
    la $t1, array       # $t1 aponta para o início do array
    addi $t2, $zero, 0

print_loop:
    bge $t2, $t0, exit_program  # se o contador >= tamanho do array, sair do loop
    lw $t3, ($t1)       # carrega o próximo elemento do array
    beq $t3, $t4, print_max_with_quotes  # se o elemento == maior elemento, imprimir com aspas
    beq $t3, $t5, print_min_with_quotes  # se o elemento == menor elemento, imprimir com aspas
    addi $v0, $zero, 1
    add $a0, $zero, $t3
    syscall
    addi $v0, $zero, 4
    la $a0, space
    syscall
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
    j print_loop

print_max_with_quotes:
    addi $v0, $zero, 4
    la $a0, quote
    syscall
    addi $v0, $zero, 1
    add $a0, $zero, $t3
    syscall
    addi $v0, $zero, 4
    la $a0, quote
    syscall
    addi $v0, $zero, 4
    la $a0, space
    syscall
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
    j print_loop

print_min_with_quotes:
    addi $v0, $zero, 4
    la $a0, quote
    syscall
    addi $v0, $zero, 1
    add $a0, $zero, $t3
    syscall
    addi $v0, $zero, 4
    la $a0, quote
    syscall
    addi $v0, $zero, 4
    la $a0, space
    syscall
    addi $t1, $t1, 4    # avança para o próximo elemento
    addi $t2, $t2, 1    # incrementa o contador
    j print_loop

exit_program:
    addi $v0, $zero, 10
    syscall
