li a7, 5
ecall
mv t0, a0
li a7, 5
ecall
mv t1, a0
li a7, 5
ecall
add t2, t0, t1
add t0, t2, a0
mv a0, t0
li a7, 1
ecall

li a7, 10
ecall