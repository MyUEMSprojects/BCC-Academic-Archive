#leia a,b,c 
addi $v0 $zero, 5
syscall
add $s0, $zero, $v0

addi $v0, $zero, 5
syscall
add $s1, $zero, $v0

addi $v0, $zero, 5
syscall
add $s2, $zero, $v0
# imprima ((a+b)/(c%b))*2

# a + b
add $t0,$s0,$s1
#c%b
div $s2,$s0
mfhi $t1

# (a+b)/(c%b)
div $t0, $t1
mflo $t2

# resultado * 2
addi $t3, $zero, 2
mult $t2,$t3
mfhi $s3