.text
.globl rand
.globl srand
.globl time
.globl main
main:
	### function preamble
	# save the base pointer
	PUSHQ %rbp
	# set new base pointer to rsp
	MOVQ %rsp, %rbp

	 # allocate 4 local variables
	SUBQ $32, %rsp

	# save callee-saved registers
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15

	### function body


	# move 20 into register
	MOVQ $20, %rbx
	MOVQ %rbx, -8(%rbp)

	# move 20 into register
	MOVQ $20, %rbx
	MOVQ %rbx, -16(%rbp)

	# move 0 into register
	MOVQ $0, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL time
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
	CALL srand
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx

	# move 0 into register
	MOVQ $0, %rbx

	# j assign 0
	MOVQ %rbx, -32(%rbp)
	# loop label
L0:
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx
	# move local 2 into %r10
	MOVQ -16(%rbp), %r10

	# j less than ysize
	CMP %r10, %rbx
	JL L2
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L3
	# true label
L2:
	MOVQ $1, %r10
	# done label
L3:
	CMP $0, %r10
	# jump to done label (break condition)
	JE L1

	# move 0 into register
	MOVQ $0, %rbx

	# i assign 0
	MOVQ %rbx, -24(%rbp)
	# loop label
L4:
	# move local 3 into %rbx
	MOVQ -24(%rbp), %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# i less than xsize
	CMP %r10, %rbx
	JL L6
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L7
	# true label
L6:
	MOVQ $1, %r10
	# done label
L7:
	CMP $0, %r10
	# jump to done label (break condition)
	JE L5
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL rand
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx

	# move 2 into register
	MOVQ $2, %r10

	# rand() MOD 2
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rdx, %r10
	MOVQ %r11, %rdx

	# move 1 into register
	MOVQ $1, %rbx

	# rand()%2 equal equal 1
	CMP %rbx, %r10
	JE L9
	MOVQ $0, %rbx
	# jump to done label (don't evaluate set to true)
	JMP L10
	# true label
L9:
	MOVQ $1, %rbx
	# done label
L10:
	CMP $0, %rbx
	JE L11

	# move '/' into register
	MOVQ $47, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_character
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# jump to done label (don't evaluate else label)
JMP L8
	# else label
L11:

	# move '\' into register
	MOVQ $92, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL print_character
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# done label
L8:

	# i INCQ 
	MOVQ -24(%rbp), %rax
	MOVQ %rax, %rbx
	INCQ %rax
	MOVQ %rax, -24(%rbp)
	# unconditional jump to next loop
JMP L4
	# done label
L5:
.data
STR0:
	.string "\n"
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

	# j INCQ 
	MOVQ -32(%rbp), %rax
	MOVQ %rax, %rbx
	INCQ %rax
	MOVQ %rax, -32(%rbp)
	# unconditional jump to next loop
JMP L0
	# done label
L1:
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
