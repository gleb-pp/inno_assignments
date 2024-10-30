main:
	li a7, 5
	ecall
	mv t0, a0
	li t2, 1
factorial:
	mul t2, t2, t0
	addi t0, t0, -1
	bgtz t0, factorial
	li a7, 1
	mv a0, t2
	ecall
	li a7, 10
	ecall