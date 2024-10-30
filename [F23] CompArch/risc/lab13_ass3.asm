.data:
	e: .float 2.71828
	pi: .float 3.14159
	
.text
li a7, 6
ecall
flw ft0, e, t0
flw ft1, pi, t0

fmul.s  fa0,  fa0, ft0
fmul.s  fa0,  fa0, ft0
fdiv.s  fa0,  fa0, ft1

li a7, 2
ecall
li a7, 10
ecall