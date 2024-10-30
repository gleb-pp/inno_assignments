.data:
	const4: .float 4.0
	pi: .float 3.14159
	
.text
li a7, 6
ecall
flw ft0, const4, t0
flw ft1, pi, t0
fmv.s fs0, fa0
fmv.s fs1, fa0

fmul.s  fs0,  fs0, fs0
fmul.s  fs0,  fs0, ft0
fmul.s  fs0,  fs0, ft1

fmul.s  fs1,  fs1, fa0
fmul.s  fs1,  fs1, fa0
fmul.s  fs1,  fs1, ft0
fmul.s  fs1,  fs1, ft1

li a7, 2
fmv.s  fa0, fs0
ecall
li a0, 10
li a7, 11
ecall
li a7, 2
fmv.s  fa0, fs1
ecall
li a7, 10
ecall