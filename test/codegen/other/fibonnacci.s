.text
.globl fib
fib:
	### function preamble
	# save the base pointer
	PUSHQ %rbp
	# set new base pointer to rsp
	MOVQ %rsp, %rbp
	# save arg 1 on the stack
	PUSHQ %rdi

	 # allocate 0 local variables
	SUBQ $0, %rsp

	# save callee-saved registers
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15

	### function body

	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 1 into register
	MOVQ $1, %r10

	# x less than 1
	CMP %r10, %rbx
	JL L1
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L2
	# true label
L1:
	MOVQ $1, %r10
	# done label
L2:
	CMP $0, %r10
	JE L3

	# move 0 into register
	MOVQ $0, %rbx

	# return statement (return 0)
	MOVQ %rbx, %rax
	### function postamble
	# restore callee-saved registers
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	# reset stack to base pointer
	MOVQ %rbp, %rsp
	# restore the old base pointer
	POPQ %rbp
	RET
	# jump to done label (don't evaluate else label)
JMP L0
	# else label
L3:
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 2 into register
	MOVQ $2, %r10

	# x less than 2
	CMP %r10, %rbx
	JL L5
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L6
	# true label
L5:
	MOVQ $1, %r10
	# done label
L6:
	CMP $0, %r10
	JE L7

	# move 1 into register
	MOVQ $1, %rbx

	# return statement (return 1)
	MOVQ %rbx, %rax
	### function postamble
	# restore callee-saved registers
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	# reset stack to base pointer
	MOVQ %rbp, %rsp
	# restore the old base pointer
	POPQ %rbp
	RET
	# jump to done label (don't evaluate else label)
JMP L4
	# else label
L7:
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 1 into register
	MOVQ $1, %r10

	# x SUBQ 1
	SUBQ %r10, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL fib
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move param 1 into %r10
	MOVQ -8(%rbp), %r10

	# move 2 into register
	MOVQ $2, %r11

	# x SUBQ 2
	SUBQ %r11, %r10
	# move arg 0 (in %r10) into %rdi
	MOVQ %r10, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL fib
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %r10

	# fib(x-1) ADDQ fib(x-2)
	ADDQ %r10, %rbx

	# return statement (return fib(x-1)+fib(x-2))
	MOVQ %rbx, %rax
	### function postamble
	# restore callee-saved registers
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	# reset stack to base pointer
	MOVQ %rbp, %rsp
	# restore the old base pointer
	POPQ %rbp
	RET
	# done label
L4:
	# done label
L0:
.globl main
main:
	### function preamble
	# save the base pointer
	PUSHQ %rbp
	# set new base pointer to rsp
	MOVQ %rsp, %rbp
	# save arg 1 on the stack
	PUSHQ %rdi
	# save arg 2 on the stack
	PUSHQ %rsi

	 # allocate 1 local variables
	SUBQ $8, %rsp

	# save callee-saved registers
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15

	### function body

.data
STR0:
	.string "Table of Fibonnacci Numbers\n"
.text
	# move STR0 into register
	LEAQ STR0, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_string
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx

	# move 0 into register
	MOVQ $0, %rbx

	# i assign 0
	MOVQ %rbx, -8(%rbp)
	# loop label
L8:
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 45 into register
	MOVQ $45, %r10

	# i less than 45
	CMP %r10, %rbx
	JL L10
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L11
	# true label
L10:
	MOVQ $1, %r10
	# done label
L11:
	CMP $0, %r10
	# jump to done label (break condition)
	JE L9
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_integer
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
.data
STR1:
	.string "t"
.text
	# move STR1 into register
	LEAQ STR1, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_string
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL fib
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_integer
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
.data
STR2:
	.string "\n"
.text
	# move STR2 into register
	LEAQ STR2, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_string
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 1 into register
	MOVQ $1, %r10

	# i ADDQ 1
	ADDQ %r10, %rbx

	# i assign i+1
	MOVQ %rbx, -8(%rbp)
	# unconditional jump to next loop
JMP L8
	# done label
L9:

	# move 0 into register
	MOVQ $0, %rbx

	# return statement (return 0)
	MOVQ %rbx, %rax
	### function postamble
	# restore callee-saved registers
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	# reset stack to base pointer
	MOVQ %rbp, %rsp
	# restore the old base pointer
	POPQ %rbp
	RET
