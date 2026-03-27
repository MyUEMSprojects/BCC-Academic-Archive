.data
   
.text

addi $v0, $zero, 9
addi $s0, $zero, 10
syscall
add $s0, $zero, $v0

add $s1, $zero, $s0
addi $t0, $s0, 16 #V+4 -> V + 4*sizeof(word)
for:
  slt $t1, $s1, $t0
  beq $t1, $zero, fim_for
  addi $v0, $zero, 5
  syscall
  sw $v0, 0($s1)
  addi $s1, $s1, 4
  j for
fim_for:

addi $v0, $zero, 10
syscall
