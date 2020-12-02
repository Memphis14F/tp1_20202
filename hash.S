# Id: hash.S

#include <sys/regdef.h>
#include <sys/syscall.h>


.abicalls

.align 2 
.globl string_hash_more
.ent string_hash_more

.align 0 
error_string: .asciiz "Assert error.\n"

.text
.align 2 

string_hash_more:
	.frame fp, 16, ra
	.set noreorder
	.cpload t9
	.set reorder

	subu	sp, sp, 8	# Movemos el Stack Pointer en 8 bytes hacia abajo (Crece)
	sw		fp, 0(sp)	# Guardamos el frame pointer en sp
	sw		gp, 4(sp)	# Guardamos el global pointer en sp+4
	move	fp, sp		# Movemos el frame pointer a donde apunta el stack pointer

	sw		a2, 16(sp)	# Almacenamos a2 en sp+16
	sw		a1, 12(sp)	# Almacenamos a1 en sp+12
	sw		a0, 8(sp)	# Almacenamos a0 en sp+8

	# Fin del stack

	move t0, a0	# Cargamos *sh a t0
	move t1, a1	# Cargamos *str a t1
	move t2, a2	# Cargamos len  a t2

	lb t3, 0(t0) 	# Cargamos en t3 el byte de t0; es decir, el primer campo de sh. (h->flag)
	lw t4, 4(t0) 	# Cargamos en t4 el segundo campo del sh. (h->hash)
	lw t5, 8(t0) 	# Cargamos en t5 el tercer campo del sh. (h->size)

	bne t3, 1, assert	# Si h->flag no es hash_init, ir al assert.

post_assertion:
	bne t3, 1, while	# Si h->flag no es INIT, ingresar al while

	li  t3, 2 			# sh->flag = STRING_HASH_MORE;
	sb t3, 0(t0)		# Guardamos el flag en el registro de C.

	lb t6, 0(t1)		# Cargamos *str en t6
	sll t6, t6, 7		# Multiplicamos t6 por 2^7 (7 posiciones en sll)
	move t4, t6			# sh->hash = (*str) << 7
	sw t4, 4(t0)		# Guardamos el hash en el registro de C.

	while: 
		lb t6, 0(t1)	# Cargamos *str en t6

		beq t6, 0, end 	# Si *str=0, finalizar
		beq t2, 0, end	# Si len = 0, finalizar

		# Si no se cumple ninguna de las condiciones:

		subu t2, t2, 1				# len = len-1
		li t7, 1000003				# Cargo el inmediato en t7 para poder multiplicar.
		mul t7, t7, t4				# t7 = hash * HASH_NUMBER
		xor t7, t7, t6				# t7 = t7 XOR *str
		addiu t1, t1, 1 			# puntero, puntero str (Sumamos 1 al *str)
		move t4, t7 				# Guardamos t7 en t4 (Nuevo valor del hash en t4).
		sw t4, 4(t0)				# Guardamos el hash en el registro de C.

		

		addiu t5, t5, 1 			# sh->size++
		sw t5, 8(t0) 				# Guardamos el size en el registro de C.

		b while 					# Retornamos al bucle



		assert:
			beq t3, 2, post_assertion	# Si h->flag es hash_more, continuar en post_assertion

			# ERROR POR NO CUMPLIR ASSERT
			li a0, 2					# Cargamos stderr como fd: a0 = 2.
			la a1, error_string			# Cargar el mensaje de error en el registro a1.
			li a2, 15					# Asignamos la cantidad de caracteres: a2 = 15.
			li v0, SYS_write			# Lanzar Error por no cumplir assert (SysPrint).
			syscall						# Ejecutar el printeo de a0.
			# EXIT Post Informe de Error
			li v0, SYS_exit				# Cargar SysExit.
			syscall						# Ejecutar Sys Exit.

		end:
			lw		fp, 0(sp)	# Cargamos el anterior frame pointer en el frame pointer
			lw		ra, 4(sp)	# Cargamos el return adress anterior al return adress
			lw		gp, 8(sp)	# Cargamos el global pointer anterior 
			add     sp, sp, 16	# Corremos el stack pointer en 16 bytes positivos
			jr ra				# Jump and link al return adress

.end string_hash_more
