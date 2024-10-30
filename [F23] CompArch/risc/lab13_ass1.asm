.data:
	const5: .float 5.0
	const9: .float 9.0
	const32: .float 32.0
	
.text
li a7, 6
ecall
flw ft0, const5, t0
flw ft1, const9, t0
flw ft2, const32, t0
fmul.s  fa0,  fa0, ft1
fdiv.s fa0, fa0, ft0
fadd.s fa0, fa0, ft2
li a7, 2
ecall
li a7, 10
ecall