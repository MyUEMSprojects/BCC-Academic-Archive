# DICAS TRAB
# MAPEAR Os Pixels em xy(função)
# endereço base do pixel - 0x10010000
# criar constantes para o trabalho

add $s0, $zero, $zero
lui $s0, 0x1001

lui $s1, 0xFF00
ori $s1, $s1, 0xFFFF 

sw $s1, 0($s0)
sw $s1, 8188($s0)
