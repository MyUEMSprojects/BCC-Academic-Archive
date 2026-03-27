# Escreva um procedimento que receba um array de inteiros e n( número de elementos)
#. O procedimento deve remover os elementos repetidos, mantendo a ordem e a primeira
# Ocorrencia do elemento. Observe que o array é passado por ponteiro e variavel
# n é passado por referencia.

# void removeRepetidos(const int *ptr, int &n);

addi $s0, $zero, 1

add $v0, $zero, $s0
add $a0, $zero, $v0
syscall

addi $v0, $zero, 10
syscall