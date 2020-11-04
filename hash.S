# Id: hash.S

#include <sys/regdef.h>
#include <sys/syscall.h>


/*
STRING_HASH_INIT 1
STRING_HASH_MORE EQU 2
STRING_HASH_DONE EQU 3
HASH_NUMBER EQU 1000003
*/

# FORMA DE DISTRIBUCION DEL STACK FraME

#	20 	  ra
#	16	  fp	
# sra

#	12	  padding
#	8	  a2		len 
#	4	  a1 		*str
# 	0 	  a0 		*sh
# aba




# .rdata
.abicalls
.text
.align 2 
.globl string_hash_more
.ent string_hash_more

string_hash_more:
	.frame fp, 24, ra
	.set noreorder
	.cpload t9
	.set reorder

	subu	sp, sp, 24	# Movemos el Stack Pointer en 24 bytes hacia abajo (Crece)
	sw		fp, 16(sp)	# Guardamos el frame pointer en sp+16
	sw		ra, 20(sp)	# Guardamos el return adress en sp+20
	move	fp, sp		# Movemos el frame pointer a donde apunta el stack pointer

	sw		a2, 8(sp)	# Almacenamos a2 en sp+8
	sw		a1, 4(sp)	# Almacenamos a1 en sp+4
	sw		a0, 0(sp)	# Almacenamos a0 en sp+0

	# Fin del stack

	move t0, a0	# Cargamos *sh a t0
	move t1, a1	# Cargamos *str a t1
	move t2, a2	# Cargamos len  a t2

	lb t3, 0(t0) 	# Cargamos en t3 el byte de t2; es decir, el primer campo de sh. (h->flag)
	lw t4, 4(t0) 	# Cargamos en t4 el segundo campo del sh. (h->hash)
	lw t5, 8(t0) 	# Cargamos en t5 el tercer campo del sh. (h->size)

	bne t3, 1, assert	# Si h->flag no es hash_init, ir al assert 1

	post_assertion: 

		bne t3, 1, while		# Si h->flag no es INIT, ingresar al while

	# HASTA ACA FUNCIONA

	li  t3, 2 		# sh->flag = STRING_HASH_MORE;
	sb t3, 0(t0)	# Guardamos el flag en el registro de C.

	lb t7, 0(t1)	# Cargamos *str en t7
	# move t6, t7		# Movemos t6 a t7
	sll t7, t7, 7	# Multiplicamos t6 por 2^7 (7 posiciones en sll)
	move t4, t7		# sh->hash = (*str) << 7
	sw t4, 4(t0)	# Guardamos el hash en el registro de C.

	while: 
		lb t7, 0(t1)	# Cargamos *str en t7

		beq t7, 0, end 	# Si *str=0, finalizar
		beq t2, 0, end	# Si len = 0, finalizar

		# Si no se cumple ninguna de las condiciones:

		subu t2, t2, 1				# len = len-1
		li t8, 1000003				# Cargo el inmediato en t8 para poder multiplicar.
		mul t8, t8, t4				# t8 = hash * HASH_NUMBER
		# mflo t8					# Cargo el resultado de la multiplicación (t8 = LO)
		xor t8, t8, t7				# t8 = t8 XOR *str
		addiu t1, t1, 1 # ver esto.  puntero, puntero str (Sumamos 1 al *str)
		move t4, t8 				# Guardamos t8 en t4 (Nuevo valor del hash en t4).
		sw t4, 4(t0)				# Guardamos el hash en el registro de C.

		

		addiu t5, t5, 1 # sh->size++
		sw t5, 8(t0) 	# Guardamos el size en el registro de C.

		b while # Retornamos al bucle



		assert:
			beq t3, 2, post_assertion	# Si h->flag es hash_more, continuar en post_assertion
			# Lanzar Error por no cumplir assert

		end:
			lw		fp, 16(sp)	# Cargamos el anterior frame pointer en el frame pointer
			lw		ra, 20(sp)	# Cargamos el return adress anterior al return adress
			addi	sp, sp, 24	# Corremos el stack pointer en 24 bytes positivos
			jr ra				# Jump and link al return adress

.end string_hash_more
