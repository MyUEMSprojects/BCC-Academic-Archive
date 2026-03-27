# .data --> armazenar constantes, variaveis estaticas etc
# .text --> programa propiamente dito...
# heap
# livre
# stack(pilha) --> passagem de parametro, alocação de variavel local etc

.data
buffer: .byte 32
.text

la $a0, buffer
addi $a1, $zero, 32
addi $v0, $zero, 8
syscall