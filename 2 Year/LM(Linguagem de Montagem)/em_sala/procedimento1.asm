# - Procedimento folha
#    $a0,...,$a3 - registradores de argumentos
#    $v0 e $v1 - registradores retorno
#    LEMBRE-SE: $s0,..., $s7 - são registradores temporarios SALVOS
#    JAL - salta p/ label e guardo o endereço do proximo inst em $ra 
#    JR - salta p/ endereço do registador

addi $a0, $zero, 10

jal somaDeUmAteN

add $a0, $zero, $v0
addi $v0, $zero, 1
syscall

addi $v0, $zero, 10
syscall

# int somaDeUmAteN(int n){
somaDeUmAteN:
#    int i;             // i->$s0
#>>>>>>>>>>>>>>>>>>>> Salvar $s0 na pilha
     addi $sp, $sp, -4
     sw $s0, 0($sp)
#    int cont=0;     // cont -> $v0
     add $v0, $zero, $zero
#    for(i=0;i<n;++i)
     add $s0, $zero, $zero
     
     for:
         slt $t0, $s0, $a0
         bne $t0, $zero, fim_for
#        cont+=i+1;
         add $v0,$v0,$s0
         addi $v0,$v0,1
         addi $s0,$s0,1
         j for
     fim_for:
# return cont;
#>>>>>>>>>>>>>> Recuperar $s0 da pilha
    lw $s0, 0($sp)
    addi $sp, $sp, 4
    jr $ra
#}	
