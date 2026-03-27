# $s0->n, $s1->V
#leia n
addi $v0,$zero,5
syscall
add $s0,$zero,$v0

#alocar array de n*4 bytes
addi $v0,$zero,9
addi $t0,$zero,4
mul $a0,$t0,$s0
syscall
add $s1,$zero,$v0

#ler array

#for(ptr = V,i=n; i>0; --i,++ptr) - i->$s2 ptr->$s3 
add $s2,$zero,$s0 #i=n
add $s3,$zero,$s1 #ptr = V
FOR:
	beq $s2,$zero,FIM_FOR
	addi $v0,$zero,5
	syscall
	sw $v0,0($s3)
	
	addi $s2,$s2,-1	#--i
	addi $s3,$s3,4  #++ptr
	j FOR
FIM_FOR:

#achar menor ($s4) e maior ($s5)

addi $s2,$s0,-1		    
addi $s3,$s1,0

lw $s4,0($s3)	#assumir que V[0] é o menor e o maior elemento
add $s5,$zero,$s4	
addi $s3,$s3,4

FOR_BUSCA:
	beq $s2,$zero,FIM_FOR_BUSCA
	lw $t0,0($s3)
	#se $t0<$s4 -> $s4 = $t0
	slt $t1,$t0,$s4
	beq $t1,$zero,FIM_IF_ME
	add $s4,$zero,$t0
	FIM_IF_ME:
	#se $t0>$s5 -> $s5 = $t0
	slt $t1,$s5,$t0
	beq $t1,$zero,FIM_IF_MA
	add $s5,$zero,$t0
	FIM_IF_MA:
	
	addi $s2,$s2,-1	
	addi $s3,$s3,4
	j FOR_BUSCA
FIM_FOR_BUSCA:

#impressão
add $s2,$zero,$s0
add $s3,$zero,$s1
FOR_IMP:
	beq $s2,$zero,FIM_FOR_IMP	
	lw $t0,0($s3)
	
	#if(*ptr == maior || *ptr==menor)
	beq $t0,$s4,IMP_ASP
	beq $t0,$s5,IMP_ASP
	j SEM_ASP
	
	IMP_ASP:
		addi $v0,$zero,11
		addi  $a0,$zero,34
		syscall
	
		addi $v0,$zero,1
		add  $a0,$zero,$t0
		syscall
	
		addi $v0,$zero,11
		addi  $a0,$zero,34
		syscall
	
	j FIM_IMP_ELEMENT
	
	SEM_ASP:
		addi $v0,$zero,1
		add  $a0,$zero,$t0
		syscall
	FIM_IMP_ELEMENT:
	
	#impressão de espaço em branco
	addi $v0,$zero,11
	addi  $a0,$zero,32
	syscall
	
	addi $s2,$s2,-1	
	addi $s3,$s3,4
	j FOR_IMP
FIM_FOR_IMP:

addi $v0,$zero,10
syscall
