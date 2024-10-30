main:
	li s0, 1  # conter of words
	li t0, 32 # symbol " "
	li t1, 10 # symbol "\n"
	li t2, 1
counter:
	li a7, 12
	ecall
	
	sub t3, a0, t0
	seqz t4, t3
	add s0, s0, t4
	sub t2, a0, t1
	bnez t2, counter
	li a7, 1
	mv a0, s0
	ecall
	li a7, 10
	ecall
