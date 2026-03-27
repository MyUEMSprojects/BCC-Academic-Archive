# SomaPar, SomaImpar
addi $s0, $zero, 0
addi $s1, $zero, 0

# i, 20
addi $s2, $zero, 0
addi $s3, $zero, 20

FOR:
slt $t0, $s3, $s2
bne $t0, $zero, FIM_FOR

   addi $t1, $zero, 2 # %2
   div $s2, $t1
   mfhi $t2

   bne $t2, $zero, ELSE
   
   add $s0, $s0, $s2
   j FIM_ELSE
   
   ELSE:
   
   add $s1, $s1, $s2
   
   FIM_ELSE:

addi $s2, $s2, 1
j FOR

FIM_FOR:

# imprima SomaPar
addi $v0, $zero, 1
add $a0, $zero, $s0
syscall

# imprima SomaImpar
addi $v0, $zero, 1
add $a0, $zero, $s1
syscall

addi $v0, $zero, 10
syscall