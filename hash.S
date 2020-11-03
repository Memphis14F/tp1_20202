#hash_more.S

# funcion no hoja 

STRING_HASH_INIT .equ 1
STRING_HASH_MORE .equ 2
STRING_HASH_DONE .equ 3
HASH_NUMBER .equ 1000003

# FORMA DE DISTRIBUCION DEL STACK FRAME

#	20 	  ra
#	16	  fp	
# sra

#	12	  padding
#	8	  a2		len 
#	4	  a1 		*str
# 	0 	  a0 		*sh
# aba

subu	sp, sp, 24	# Movemos el Stack Pointer en 24 bytes hacia abajo (Crece)
sw		fp, 16(sp)	# Guardamos el frame pointer en sp+16
sw		ra, 20(sp)	# Guardamos el return adress en sp+20
move	fp, sp		# Movemos el frame pointer a donde apunta el stack pointer

sw		a2, 8(sp)	# Almacenamos a2 en sp+8
sw		a1, 4(sp)	# Almacenamos a1 en sp+4
sw		a0, 0(sp)	# Almacenamos a0 en sp+0

# Fin del stack

move t0, a0	# Cargamos len a t0
move t1, a1	# Cargamos *str a t1
move t2, a2	# Cargamos *sh  a t2

lb t3, 0(t2) 	# Cargamos en t3 el byte de t2; es decir, el primer campo de sh. (h->flag)
lw t4, 4(t2) 	# Cargamos en t4 el segundo campo del sh. (h->hash)
lw t5, 8(t2) 	# Cargamos en t5 el tercer campo del sh. (h->size)

bne t3, STRING_HASH_INIT, assert	# Si h->flag no es hash_init, ir al assert 1

post_assertion: 

	bne t3, STRING_HASH_INIT, while		# Si h->flag no es INIT, ingresar al while

sb  t2, STRING_HASH_MORE 	# sh->flag = STRING_HASH_MORE;

move t6, t1		# Movemos t6 a t1
sll t6, t6, 7	# Multiplicamos t6 por 14 (7 posiciones en sll)
sw t4, t6		# sh->hash = (*str) << 7

while: 

	lb t7, 0(t1)	# Cargamos *str en t7

	beq t7, 0, end 	# Si *str=0, finalizar
	beq t0, 0, end	# Si len = 0, finalizar

	# Si no se cumple ninguna de las condiciones:

	subu t0, t0, 1				# len = len-1
	mult t8, t4, HASH_NUMBER	# t8 = hash * HASH_NUMBER
	xor t8, t8, t1				# t8 = t8 XOR *str
	sw t4, t8 					# Guardamos t8 en t4 (Nuevo valor del hash en t4).

	addiu t1, 1(t1) # ver esto.  puntero, puntero str (Sumamos 1 al *str)

	addiu t5, 1(t5) # sh->size++

	b while # Retornamos al bucle



assert:
	beq t3, STRING_HASH_MORE, post_assertion	# Si h->flag es hash_more, continuar en post_assertion
	# Lanzar Error por no cumplir assert

end:
	lw		fp, 16(sp)	# Cargamos el anterior frame pointer en el frame pointer
	lw		ra, 24(sp)	# Cargamos el return adress anterior al return adress
	addi	sp, sp, 24	# Corremos el stack pointer en 24 bytes positivos
	jr ra				# Jump and link al return adress





