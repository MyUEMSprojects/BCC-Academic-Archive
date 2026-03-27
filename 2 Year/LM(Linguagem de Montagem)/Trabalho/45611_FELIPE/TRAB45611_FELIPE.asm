.data
filename: .asciiz "arquivo.txt"     # Nome do arquivo
buffer:   .space 256                # Buffer para armazenar cada linha

#Nome: Felipe Echeverria Vilhalva RGM: 45611
#TAMANHO MAXIMO (X,Y): (63,31)
# o arquivo.txt deve estar no mesmo diretório do Mars.jar!
# Unit Width in Pixels: 8
# Unit Height in Pixels: 8
# Exemplo de arquivo: 2,29 4,29 4,26 2,26 EOF
# desenha apenas uma figura por arquivo!
# se nao colocar o EOF no final do arquivo, ele nao funfa
# tirei a cor no inicio do arquivo por motivos de otimização(nao vai ter diferença)
# se se pintasse...
# problemas: para diferenças muito grandes de (x,y) talvez as linhas nao estejam completas :(
# o programa pequenas pseudo-instrucoes pelo motivo de que eu nao consegui fazer algumas coisas
# - sem uso delas

.text
main:

    add $s0, $zero, $zero
    lui $s0, 0x1001
    
    # Inicializa cor RGB
    lui $s1, 0xFF00
    ori $s1, $s1, 0xFFFF            # Define a cor inicial como azul em $s2
    
    #sw $s1, 3964($s0)	
    
    addi $s2, $zero, 0 #contador para ver se o (x,y) eh inicial

    # Inicializa a primeira posicao de x e y para coordenada inicial
    addi $s3, $zero, 0    # x inicial
    addi $s4, $zero, 0    # y inicial
    
    addi $s5, $zero, 0    # inicializa o x anterior
    addi $s6, $zero, 0    # inicializa o y anterior

    # Abrir o arquivo para leitura
    addi $v0, $zero, 13                      # Syscall para abrir arquivo
    la $a0, filename                         # Nome do arquivo
    addi $a1, $zero, 0                       # Modo de leitura (0 = leitura)
    syscall
    add $s7, $zero, $v0                      # Armazena o descritor de arquivo em $s0

read_line:
    # Ler uma linha do arquivo
    addi $v0, $zero, 14                      # Syscall para ler arquivo
    add $a0, $zero, $s7                   # Descritor de arquivo
    la $a1, buffer                  # Buffer para armazenar a linha
    addi $a2, $zero, 256                     # Tamanho máximo da linha
    syscall
    slt $t0, $v0, $zero
    bne $t0, $zero, end_of_file
    beq $v0, $zero, end_of_file
    

    # Processar a linha para obter coordenadas
    la $t0, buffer                  # Ponteiro para o inicio da linha
    jal process_coordinates         # Chama a funcao para processar coordenadas

    j read_line                     # Ler a proxima linha
    
end_of_file:
    # Fechar o arquivo
    addi $v0, $zero, 16                      # Syscall para fechar arquivo
    add $a0, $zero, $s7                   # Descritor de arquivo
    syscall

    # Finalizar o programa
    addi $v0, $zero, 10                      # Syscall para encerrar
    syscall

# Funcao para processar coordenadas na linha atual
process_coordinates:
    addi $t6, $zero, 0                    # Alternador de coordenadas: 0 = x, 1 = y
    add $t1, $zero, $zero                 # Limpa acumulador de numero
    addi $t7, $zero, 0                    # Flag para indicar se a coordenada inicial foi armazenada

process_char:
      lb $t3, 0($t0)                  # Carrega o proximo caractere da linha
      beq $t3, 0, end_process_line    # Fim da linha (caractere NULL)
    
      # Verificar se o caractere eh menor que '0' (48)
      addi $t4, $zero, 48               # ASCII de '0'
      slt  $t5, $t3, $t4                # $t5 = 1 se $t0 < $t1, caso contrario $t5 = 0
      bne  $t5, $zero, handle_separator # Se $t2 != 0, nao eh digito
 
      # Verificar se o caractere eh maior que '9' (57)
      addi $t4, $zero, 57               # ASCII de '9'
      slt  $t5, $t4, $t3                # $t2 = 1 se $t3 < $t0, caso contrÃ¡rio $t2 = 0
      bne  $t5, $zero, handle_separator # Se $t2 != 0, nao eh digito
      
      # se chegou aqui, eh um digito; converte e acumula
      addi $t4, $zero, 48   # Carrega o valor ASCII '0' em $t4
      sub  $t3, $t3, $t4    # Converte o caractere para numero subtraindo '0'
      
      mul $t1, $t1, 10      # Multiplica o acumulador por 10
      add $t1, $t1, $t3     # Adiciona o novo Digito ao acumulador
    
      j next_char                     # Vai para o proximo caractere

handle_separator:
    # Se estamos acumulando um numero, processa e decide se eh x ou y
    beq $t6, 0, store_x             # Se $t6 == 0, armazena como coordenada x
    j store_y                       # Caso contrario, armazena como coordenada y

store_x:
    # Armazena o valor x no registrador $t2
    add $t2, $zero, $t1                   # Coloca o valor de x em $t2
    j continue_y                    # Pula para armazenar y

continue_y:
    addi $t1, $zero, 0                       # Limpa acumulador para proximo numero
    addi $t6, $zero, 1                       # Define o proximo numero como y
    j next_char

store_y:
    # Armazena o valor y no registrador $t4
    add $t4, $zero, $t1                   # Coloca o valor de y em $t4
    
    # Imprime o par (x, y) para verificacao
    j print_coord                   # Pula para a impressão do par (x, y)

print_coord:
   bne $t2, $zero, EOF
        bne $t4, $zero, EOF
            # se for fim do arquivo
            
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
          # if(xi < x_ant && yi < y_ant) (x++, y++)
          slt $t1, $t8, $s3
          beq $t1, $zero, fim_se1_1
              slt $t1, $t9, $s4
              beq $t1, $zero, fim_se1_1
                   enquanto1_1:
                       slt $t1, $t8, $s3
                       beq $t1, $zero, fim_enquanto1_1
                           slt $t1, $t9, $s4
                           beq $t1, $zero, fim_enquanto1_1
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, 1 # xi++
                               addi $t9, $t9, 1 # yi++
                               j enquanto1_1
                  fim_enquanto1_1:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado  
                  j fim_se1_1
          fim_se1_1:
          
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
  
      # if(x_ant < xi && yi < y_ant) (xi-, y+)
          slt $t1, $s3, $t8
          beq $t1, $zero, fim_se2_2
              slt $t1, $t9, $s4 
              beq $t1, $zero, fim_se2_2                       
                   enquanto2_2:
                       slt $t1, $s3, $t8 
                       beq $t1, $zero, fim_enquanto2_2
                           slt $t1, $t9, $s4  
                           beq $t1, $zero, fim_enquanto2_2
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               addi $t9, $t9, 1 # yi++
                               j enquanto2_2
                  fim_enquanto2_2:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se2_2
      fim_se2_2:
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
      # if(xi < x_ant && y_ant < yi) (xi+,yi-)
          slt $t1, $t8, $s3
          beq $t1, $zero, fim_se3_3
              slt $t1, $s4, $t9
              beq $t1, $zero, fim_se3_3
                   enquanto3_3:
                       slt $t1, $t8, $s3 
                       beq $t1, $zero, fim_enquanto3_3
                           slt $t1, $s4, $t9  
                           beq $t1, $zero, fim_enquanto3_3
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, 1 # xi++
                               addi $t9, $t9, -1 # yi--
                               j enquanto3_3
                  fim_enquanto3_3:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se3_3
      fim_se3_3:
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
      # if(x_ant < xi && y_ant < yi) (xi-,yi-)
          slt $t1, $s3, $t8
          beq $t1, $zero, fim_se4_4
              slt $t1, $s4, $t9 
              beq $t1, $zero, fim_se4_4
                   enquanto4_4:
                       slt $t1, $s3, $t8 
                       beq $t1, $zero, fim_enquanto4_4
                           slt $t1, $s4, $t9  
                           beq $t1, $zero, fim_enquanto4_4
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               addi $t9, $t9, -1 # yi--
                               j enquanto4_4
                  fim_enquanto4_4:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se4_4
      fim_se4_4:
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
      # if(xi == x_ant && yi < y_ant) (xi, yi++)
          bne $t8, $s3, fim_se5_5
              slt $t1, $t9, $s4 
              beq $t1, $zero, fim_se5_5
                   enquanto5_5:
                       slt $t1, $t9, $s4 
                       beq $t1, $zero, fim_enquanto5_5
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t9, $t9, 1 # yi++
                               j enquanto5_5
                  fim_enquanto5_5:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se5_5
      fim_se5_5:
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
      # if(xi == x_ant && y_ant < yi) (xi, yi-)
          bne $t8, $s3, fim_se6_6
              slt $t1, $s4, $t9 
              beq $t1, $zero, fim_se6
                   enquanto6_6:
                       slt $t1, $s4, $t9 
                       beq $t1, $zero, fim_enquanto6_6
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t9, $t9, -1 # yi--
                               j enquanto6_6
                  fim_enquanto6_6:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se6_6
      fim_se6_6:

      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
          # if(yi == y_ant && xi < x_ant) (xi+, yi)
          bne $t9, $s4, fim_se7_7
              slt $t1, $t8, $s3  
              beq $t1, $zero, fim_se7_7
                   enquanto7_7:
                       slt $t1, $t8, $s3 
                       beq $t1, $zero, fim_enquanto7_7
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, 1 # xi++
                               j enquanto7_7
                  fim_enquanto7_7:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se7_7
      fim_se7_7:
      add $t8, $zero, $s5 # guarda x_ant
      add $t9, $zero, $s6 # guarda y_ant
      
      # if(yi == y_ant && x_ant < xi) (xi-,yi)
          bne $t9, $s4, fim_se8_8
              slt $t1, $s3, $t8  
              beq $t1, $zero, fim_se8_8
                   enquanto8_8:
                       slt $t1, $s3, $t8 
                       beq $t1, $zero, fim_enquanto8_8
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               j enquanto8_8
                  fim_enquanto8_8:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se8_8
      fim_se8_8:
      
      # Limpa o acumulador e reseta alternador para próxima coordenada
      addi $t1, $zero, 0                       # Limpa acumulador para próximo número
      addi $t6, $zero, 0                       # Reseta para que o próximo seja x
      add $t2, $zero, $zero                    # Limpa x
      add $t4, $zero, $zero                    # Limpa y
      j next_char
   EOF:
   beq $s2, $zero, eh_inicial
      # faça alguma coisa
      add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      
          # if(xi < x_ant && yi < y_ant) (x++, y++)
          slt $t1, $t2, $s5
          beq $t1, $zero, fim_se1
              slt $t1, $t4, $s6
              beq $t1, $zero, fim_se1
                   enquanto1:
                       slt $t1, $t8, $s5
                       beq $t1, $zero, fim_enquanto1
                           slt $t1, $t9, $s6
                           beq $t1, $zero, fim_enquanto1
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               j enquanto1
                  fim_enquanto1:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado  
                  j fim_se1
          fim_se1:
          
                add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
  
      # if(x_ant < xi && yi < y_ant) (xi-, y+)
          slt $t1, $s5, $t2
          beq $t1, $zero, fim_se2
              slt $t1, $t4, $s6 
              beq $t1, $zero, fim_se2
                   enquanto2:
                       slt $t1, $s5, $t8 
                       beq $t1, $zero, fim_enquanto2
                           slt $t1, $t9, $s6  
                           beq $t1, $zero, fim_enquanto2
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               addi $t9, $t9, 1 # yi++
                               j enquanto2
                  fim_enquanto2:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se2
      fim_se2:
            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      # if(xi < x_ant && y_ant < yi) (xi+,yi-)
          slt $t1, $t2, $s5
          beq $t1, $zero, fim_se3
              slt $t1, $s6, $t4 
              beq $t1, $zero, fim_se3
                   enquanto3:
                       slt $t1, $t8, $s5 
                       beq $t1, $zero, fim_enquanto3
                           slt $t1, $s6, $t9  
                           beq $t1, $zero, fim_enquanto3
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, 1 # xi++
                               addi $t9, $t9, -1 # yi--
                               j enquanto3
                  fim_enquanto3:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se3
      fim_se3:
            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      
      # if(x_ant < xi && y_ant < yi) (xi-,yi-)
          slt $t1, $s5, $t2
          beq $t1, $zero, fim_se4
              slt $t1, $s6, $t4 
              beq $t1, $zero, fim_se4
                   enquanto4:
                       slt $t1, $s5, $t8 
                       beq $t1, $zero, fim_enquanto4
                           slt $t1, $s6, $t9  
                           beq $t1, $zero, fim_enquanto4
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               addi $t9, $t9, -1 # yi--
                               j enquanto4
                  fim_enquanto4:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se4
      fim_se4:
            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      # if(xi == x_ant && yi < y_ant) (xi, yi++)
          bne $t2, $s5, fim_se5
              slt $t1, $t4, $s6 
              beq $t1, $zero, fim_se5
                   enquanto5:
                       slt $t1, $t9, $s6 
                       beq $t1, $zero, fim_enquanto5
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t9, $t9, 1 # yi++
                               j enquanto5
                  fim_enquanto5:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se5
      fim_se5:
            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      # if(xi == x_ant && y_ant < yi) (xi, yi-)
          bne $t2, $s5, fim_se6
              slt $t1, $s6, $t4 
              beq $t1, $zero, fim_se6
                   enquanto6:
                       slt $t1, $s6, $t9 
                       beq $t1, $zero, fim_enquanto6
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t9, $t9, -1 # yi--
                               j enquanto6
                  fim_enquanto6:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se6
      fim_se6:

            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
          # if(yi == y_ant && xi < x_ant) (xi+, yi)
          bne $t4, $s6, fim_se7
              slt $t1, $t2, $s5  
              beq $t1, $zero, fim_se7
                   enquanto7:
                       slt $t1, $t8, $s5 
                       beq $t1, $zero, fim_enquanto7
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, 1 # xi++
                               j enquanto7
                  fim_enquanto7:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se7
      fim_se7:
            add $t8, $zero, $t2 # guarda xi
      add $t9, $zero, $t4 # guarda yi
      # if(yi == y_ant && x_ant < xi) (xi-,yi)
          bne $t4, $s6, fim_se8
              slt $t1, $s5, $t2  
              beq $t1, $zero, fim_se8
                   enquanto8:
                       slt $t1, $s5, $t8 
                       beq $t1, $zero, fim_enquanto8
                               
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                               
                               addi $t8, $t8, -1 # xi--
                               j enquanto8
                  fim_enquanto8:
                               addi $t1, $zero, 4
                               addi $t6, $zero, 256
                               mul $t5, $t8, $t1   # [xi * 4]
                               mul $t6, $t9, $t6   # [yi * 256]
                               add $t6, $t6, $t5   # Endereço do bloco
     
                               # Pintar o bloco
                               # Definindo a cor a ser usada
                               addi $t7, $zero, 0xFF0000    # Cor vermelha (por exemplo)
     
                               # Escreve a cor no bitmap
                               add $t5, $s0, $t6   # Adiciona o endereço base
                               sw $t7, 0($t5)      # Armazena a cor no endereço calculado
                  j fim_se8
      fim_se8:
      
      add $s5, $zero, $t2 # coloca o x atual no x anterior $s5
      add $s6, $zero, $t4 # coloca o y atual no y anterior $s5
      
      # Limpa o acumulador e reseta alternador para próxima coordenada
      addi $t1, $zero, 0                       # Limpa acumulador para próximo número
      addi $t6, $zero, 0                       # Reseta para que o próximo seja x
      add $t2, $zero, $zero                    # Limpa x
      add $t4, $zero, $zero                    # Limpa y
      j next_char
      
eh_inicial:
   add $s3, $zero, $t2 # coloca o primeiro x no $s3
   add $s4, $zero, $t4 # coloca o primeiro y no $s4
   addi $s2, $zero, 1 #  o proximo nao he mais inicial
         
   add $s5, $zero, $t2 # coloca o x atual no x anterior $s5
   add $s6, $zero, $t4 # coloca o y atual no y anterior $s5
   
   # Limpa o acumulador e reseta alternador para próxima coordenada
   addi $t1, $zero, 0                       # Limpa acumulador para próximo número
   addi $t6, $zero, 0                       # Reseta para que o próximo seja x
   add $t2, $zero, $zero                    # Limpa x
   add $t4, $zero, $zero                    # Limpa y
   j next_char
next_char:
    addi $t0, $t0, 1                # Avança para o proximo caractere na linha
    j process_char                  # Continua processando a linha

end_process_line:
    jr $ra                           # Retorna para o chamador
