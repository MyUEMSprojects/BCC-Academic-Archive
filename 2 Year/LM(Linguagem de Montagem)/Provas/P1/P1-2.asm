# ler n
addi $v0, $zero, 5
syscall
add $s0, $zero, $v0

# alocar memória n * 4
addi $v0, $zero, 9
addi $t0, $zero, 4
mul $a0, $s0, $t0
syscall
add $s1, $zero, $v0

add $s2, $zero, $s1 # ptr = V
addi $s3, $zero, 0 # i = 0
addi $s4, $zero, 0 # j = 0

# for para escrever
FOR_ESCRITA:
    slt $t0, $s3, $s0
    beq $t0, $zero, FIM_FOR_ESCRITA

    addi $v0, $zero, 5
    syscall
    add $t1, $zero, $v0
    
    sw $t1, 0($s2)
    
    addi $s3, $s3, 1
    addi $s2, $s2, 4
    j FOR_ESCRITA
FIM_FOR_ESCRITA:

# Resetando o ponteiro e índice para o início do array
add $s2, $zero, $s1 # ptr = V
addi $s3, $zero, 0 # i = 0

addi $s5, $s0, -1 # n - 1

# for(j = 0; j < n; j++)
FOR_BUBBLE:
    slt $t0, $s4, $s0
    beq $t0, $zero, FIM_FOR_BUBBLE
     
    add $s2, $zero, $s1 # ptr = V
    addi $s3, $zero, 0 # i = 0          
     
    # for(ptr = V, i = 0; i < n - 1; ++i, ++ptr)
    FOR_BUBBLE_INTERNO:
        slt $t1, $s3, $s5
        beq $t1, $zero, FIM_FOR_BUBBLE_INTERNO
         
        addi $s6, $s2, 4 # (ptr+1)
        lw $t2, 0($s2) # *ptr
        lw $t3, 0($s6) # *(ptr+1)
         
        # if(*ptr > *(ptr+1)
        slt $t4, $t3, $t2
        beq $t4, $zero, FIM_IF
            lw $t5, 0($s2) # *ptr
            lw $t6, 0($s6) # *(ptr+1)
             
            add $t7, $zero, $t5 # aux = *ptr
            sw $t6, 0($s2) # *ptr = *(ptr+1) 
            sw $t7, 0($s6) # *(ptr+1) = aux
        FIM_IF:
         
        addi $s3, $s3, 1
        addi $s2, $s2, 4
        j FOR_BUBBLE_INTERNO
    FIM_FOR_BUBBLE_INTERNO:
     
    addi $s4, $s4, 1
    j FOR_BUBBLE
FIM_FOR_BUBBLE:

# Resetando o ponteiro e índice para o início do array para leitura
add $s2, $zero, $s1 # ptr = V
addi $s3, $zero, 0 # i = 0   

FOR_LEITURA:
    slt $t0, $s3, $s0
    beq $t0, $zero, FIM_LEITURA_FOR
    
    lw $t1, 0($s2)
    
    addi $v0, $zero, 1
    add $a0, $zero, $t1
    syscall
    
    addi $s3, $s3, 1
    addi $s2, $s2, 4
    j FOR_LEITURA
FIM_LEITURA_FOR:

addi $v0, $zero, 10
syscall
