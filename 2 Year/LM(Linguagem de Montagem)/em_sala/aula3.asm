#lui $s0, 0x1
#addiu $s0, $s0, 0x1

#int x; // x >> $s0

#if(x) {
# x =0;
#}
#x++

#if(x)
beq $s0, $zero, FIM_DO_IF
#x=0
addi $s0, $zero, 0
FIM_DO_IF:
#x++
addi $s0, $s0, 1

###############################
#int x, y; // x >> $s0   y >> $s1
#if(x < y)
slt $t0, $s0, $s1 ## $t0 = x<y?1:0
beq $t0, $zero, FIM_DO_IF
     addi $s0, $zero, 0

FIM_DO_IF:
addi $s0, $s0, 1

###############################
#int x; // x >> $s0
slti $t0, $s0, 3 ## $t0 = x<3?1:0
beq $t0, $zero, FIM_DO_IF
     addi $s0, $zero, 0

FIM_DO_IF:
addi $s0, $s0, 1

#################
# if(3 < x)
addi $t0, $zero, 3
slt $t0, $t0, $s0


