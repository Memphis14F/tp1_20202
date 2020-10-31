hash_more.S

funcion no hoja 


STRING_HASH_INIT .equ 1
TRING_HASH_MORE  .equ 2
HASH_NUMBER .equ 1000003

	20 	  ra
sra 16	  fp	
	12	  padding
	8	  a2	#  len 
	4	  a1 	# *str
aba	0 	  a0 	# *sh


subu	sp, sp, 24
sw		fp, 16(sp)
sw		ra, 24(sp)
move	fp, sp

sw		a2, 8(sp)
sw		a1, 4(sp)
sw		a0, 0(sp)

# Fin del stack

move t0, a0	  # len
move t1, a1	  #*str
move t2, a2   #*sh 

lb t3, 0(t2) #load byte	lw load word 32 lh sh->flag
lw t4, 4(t2) # sh->hash
lw t5, 8(t2) # sh->size 

bnq t3, STRING_HASH_INIT, assert1
bnq t3, STRING_HASH_MORE, assert2


bnq t3, STRING_HASH_INIT, while

sb  t2, STRING_HASH_MORE 	#sh->flag = STRING_HASH_MORE;

move t6, t1 
sll t6, t6, 7
sw t4, t6	#sh->hash = (*str) << 7;

while: 

	lb t7, 0(t1)  #*str

	beq t7, 0, end 
	beq t0, 0, end

	subu t0, t0, 1
	mult t8, t4, HASH_NUMBER
	xor t8, t8, t1
	sw t4, t8 

	addiu t1, 1(t1) #ver esto.  puntero, puntero str

	addiu t5, 1(t5) #sh->size++;

	b while 



assert1:

assert2:  

end:
	lw		fp, 16(sp)
	lw		ra, 24(sp)
	addi	sp, sp, 24
	jr ra





