.text
.globl gfx_open
.globl gfx_point
.globl gfx_line
.globl gfx_color
.globl gfx_clear
.globl gfx_clear_color
.globl gfx_wait
.globl gfx_xpos
.globl gfx_ypos
.globl gfx_xsize
.globl gfx_ysize
.globl gfx_event_waiting
.globl gfx_flush
.globl usleep
.globl rand
.globl draw_box
draw_box:
	### function preamble
	# save the base pointer
	PUSHQ %rbp
	# set new base pointer to rsp
	MOVQ %rsp, %rbp
	# save arg 1 on the stack
	PUSHQ %rdi
	# save arg 2 on the stack
	PUSHQ %rsi
	# save arg 3 on the stack
	PUSHQ %rdx

	 # allocate 0 local variables
	SUBQ $0, %rsp

	# save callee-saved registers
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15

	### function body


	# move 255 into register
	MOVQ $255, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi

	# move 255 into register
	MOVQ $255, %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi

	# move 255 into register
	MOVQ $255, %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_color
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# x ADDQ size
	ADDQ %r10, %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move arg 3 (in %rbx) into %rcx
	MOVQ %rbx, %rcx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_line
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# x ADDQ size
	ADDQ %r10, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# x ADDQ size
	ADDQ %r10, %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# y ADDQ size
	ADDQ %r10, %rbx
	# move arg 3 (in %rbx) into %rcx
	MOVQ %rbx, %rcx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_line
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# x ADDQ size
	ADDQ %r10, %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# y ADDQ size
	ADDQ %r10, %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# y ADDQ size
	ADDQ %r10, %rbx
	# move arg 3 (in %rbx) into %rcx
	MOVQ %rbx, %rcx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_line
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move param 3 into %r10
	MOVQ -24(%rbp), %r10

	# y ADDQ size
	ADDQ %r10, %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi
	# move param 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# move param 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move arg 3 (in %rbx) into %rcx
	MOVQ %rbx, %rcx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_line
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
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
.globl main
main:
	### function preamble
	# save the base pointer
	PUSHQ %rbp
	# set new base pointer to rsp
	MOVQ %rsp, %rbp

	 # allocate 9 local variables
	SUBQ $72, %rsp

	# save callee-saved registers
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15

	### function body


	# move 100 into register
	MOVQ $100, %rbx
	MOVQ %rbx, -8(%rbp)

	# move 500 into register
	MOVQ $500, %rbx
	MOVQ %rbx, -16(%rbp)

	# move 500 into register
	MOVQ $500, %rbx
	MOVQ %rbx, -24(%rbp)
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move local 2 into %r10
	MOVQ -16(%rbp), %r10

	# precision multiply xsize
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# move 2 into register
	MOVQ $2, %rbx

	# precision*xsize DIV 2
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -32(%rbp)
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx
	# move local 3 into %r10
	MOVQ -24(%rbp), %r10

	# precision multiply ysize
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# move 2 into register
	MOVQ $2, %rbx

	# precision*ysize DIV 2
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -40(%rbp)
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 3 into register
	MOVQ $3, %r10

	# precision multiply 3
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -48(%rbp)
	# move local 1 into %rbx
	MOVQ -8(%rbp), %rbx

	# move 5 into register
	MOVQ $5, %r10
	NEG %r10

	# precision multiply -5
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -56(%rbp)

	# move 100 into register
	MOVQ $100, %rbx
	MOVQ %rbx, -64(%rbp)
	# move local 2 into %rbx
	MOVQ -16(%rbp), %rbx
	# move arg 0 (in %rbx) into %rdi
	MOVQ %rbx, %rdi
	# move local 3 into %rbx
	MOVQ -24(%rbp), %rbx
	# move arg 1 (in %rbx) into %rsi
	MOVQ %rbx, %rsi
.data
STR0:
	.string "Bounce!"
.text
	# move STR0 into register
	LEAQ STR0, %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_open
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# loop label
L0:
.data
STR1:
	.string "x: "
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
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx
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
	.string " y: "
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
	# move local 5 into %rbx
	MOVQ -40(%rbp), %rbx
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
STR3:
	.string " vx: "
.text
	# move STR3 into register
	LEAQ STR3, %rbx
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
	# move local 6 into %rbx
	MOVQ -48(%rbp), %rbx
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
STR4:
	.string " vy: "
.text
	# move STR4 into register
	LEAQ STR4, %rbx
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
	# move local 7 into %rbx
	MOVQ -56(%rbp), %rbx
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
STR5:
	.string "\n"
.text
	# move STR5 into register
	LEAQ STR5, %rbx
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
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_event_waiting
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	CMP $0, %rbx
	JE L2
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_wait
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx

	# c assign gfx_wait()
	MOVQ %rbx, -72(%rbp)
	# move local 9 into %rbx
	MOVQ -72(%rbp), %rbx

	# move 'q' into register
	MOVQ $113, %r10

	# c equal equal 'q'
	CMP %r10, %rbx
	JE L4
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L5
	# true label
L4:
	MOVQ $1, %r10
	# done label
L5:
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
	# done label
L3:
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_xpos
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# gfx_xpos() multiply precision
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# x assign gfx_xpos()*precision
	MOVQ %r10, -32(%rbp)
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_ypos
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# gfx_ypos() multiply precision
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# y assign gfx_ypos()*precision
	MOVQ %r10, -40(%rbp)

	# move 5 into register
	MOVQ $5, %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# 5 multiply precision
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# vx assign 5*precision
	MOVQ %r10, -48(%rbp)
	# done label
L2:
	# move local 7 into %rbx
	MOVQ -56(%rbp), %rbx

	# move 1 into register
	MOVQ $1, %r10
	# move local 1 into %r11
	MOVQ -8(%rbp), %r11

	# 1 multiply precision
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11

	# vy ADDQ 1*precision
	ADDQ %r11, %rbx

	# vy assign vy+1*precision
	MOVQ %rbx, -56(%rbp)
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx

	# move 0 into register
	MOVQ $0, %r10

	# x less than 0
	CMP %r10, %rbx
	JL L7
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L8
	# true label
L7:
	MOVQ $1, %r10
	# done label
L8:
	# move local 6 into %rbx
	MOVQ -48(%rbp), %rbx

	# move 0 into register
	MOVQ $0, %r11

	# vx less than 0
	CMP %r11, %rbx
	JL L9
	MOVQ $0, %r11
	# jump to done label (don't evaluate set to true)
	JMP L10
	# true label
L9:
	MOVQ $1, %r11
	# done label
L10:

	# x<0 and vx<0
	# check that left expr is false
	CMP $0, %r10
	JE L12
	# check that right expr is false
	CMP $0, %r11
	JNE L11
	# false label
L12:
	MOVQ $0, %r11
	# jump to done label (don't evaluate set to true)
	JMP L13
	# true label
L11:
	MOVQ $1, %r11
	# done label
L13:
	CMP $0, %r11
	JE L6

	# move 9 into register
	MOVQ $9, %rbx
	NEG %rbx
	# move local 6 into %r10
	MOVQ -48(%rbp), %r10

	# -9 multiply vx
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# move 10 into register
	MOVQ $10, %rbx

	# -9*vx DIV 10
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx

	# vx assign -9*vx/10
	MOVQ %rbx, -48(%rbp)
	# done label
L6:
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx
	# move local 2 into %r10
	MOVQ -16(%rbp), %r10
	# move local 1 into %r11
	MOVQ -8(%rbp), %r11

	# xsize multiply precision
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11

	# x greater than (xsize*precision)
	CMP %r11, %rbx
	JG L15
	MOVQ $0, %r11
	# jump to done label (don't evaluate set to true)
	JMP L16
	# true label
L15:
	MOVQ $1, %r11
	# done label
L16:
	# move local 6 into %rbx
	MOVQ -48(%rbp), %rbx

	# move 0 into register
	MOVQ $0, %r10

	# vx greater than 0
	CMP %r10, %rbx
	JG L17
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L18
	# true label
L17:
	MOVQ $1, %r10
	# done label
L18:

	# x>(xsize*precision) and vx>0
	# check that left expr is false
	CMP $0, %r11
	JE L20
	# check that right expr is false
	CMP $0, %r10
	JNE L19
	# false label
L20:
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L21
	# true label
L19:
	MOVQ $1, %r10
	# done label
L21:
	CMP $0, %r10
	JE L14

	# move 9 into register
	MOVQ $9, %rbx
	NEG %rbx
	# move local 6 into %r10
	MOVQ -48(%rbp), %r10

	# -9 multiply vx
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# move 10 into register
	MOVQ $10, %rbx

	# -9*vx DIV 10
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx

	# vx assign -9*vx/10
	MOVQ %rbx, -48(%rbp)
	# done label
L14:
	# move local 5 into %rbx
	MOVQ -40(%rbp), %rbx
	# move local 3 into %r10
	MOVQ -24(%rbp), %r10
	# move local 1 into %r11
	MOVQ -8(%rbp), %r11

	# ysize multiply precision
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11

	# y greater than (ysize*precision)
	CMP %r11, %rbx
	JG L23
	MOVQ $0, %r11
	# jump to done label (don't evaluate set to true)
	JMP L24
	# true label
L23:
	MOVQ $1, %r11
	# done label
L24:
	# move local 7 into %rbx
	MOVQ -56(%rbp), %rbx

	# move 0 into register
	MOVQ $0, %r10

	# vy greater than 0
	CMP %r10, %rbx
	JG L25
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L26
	# true label
L25:
	MOVQ $1, %r10
	# done label
L26:

	# y>(ysize*precision) and vy>0
	# check that left expr is false
	CMP $0, %r11
	JE L28
	# check that right expr is false
	CMP $0, %r10
	JNE L27
	# false label
L28:
	MOVQ $0, %r10
	# jump to done label (don't evaluate set to true)
	JMP L29
	# true label
L27:
	MOVQ $1, %r10
	# done label
L29:
	CMP $0, %r10
	JE L22

	# move 9 into register
	MOVQ $9, %rbx
	NEG %rbx
	# move local 7 into %r10
	MOVQ -56(%rbp), %r10

	# -9 multiply vy
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10

	# move 10 into register
	MOVQ $10, %rbx

	# -9*vy DIV 10
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx

	# vy assign -9*vy/10
	MOVQ %rbx, -56(%rbp)
	# done label
L22:
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx
	# move local 6 into %r10
	MOVQ -48(%rbp), %r10
	# move local 8 into %r11
	MOVQ -64(%rbp), %r11

	# vx multiply deltat
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# vx*deltat DIV precision
	MOVQ %rdx, %r12
	MOVQ %r11, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r12, %rdx

	# x ADDQ vx*deltat/precision
	ADDQ %r10, %rbx

	# x assign x+vx*deltat/precision
	MOVQ %rbx, -32(%rbp)
	# move local 5 into %rbx
	MOVQ -40(%rbp), %rbx
	# move local 7 into %r10
	MOVQ -56(%rbp), %r10
	# move local 8 into %r11
	MOVQ -64(%rbp), %r11

	# vy multiply deltat
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# vy*deltat DIV precision
	MOVQ %rdx, %r12
	MOVQ %r11, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r12, %rdx

	# y ADDQ vy*deltat/precision
	ADDQ %r10, %rbx

	# y assign y+vy*deltat/precision
	MOVQ %rbx, -40(%rbp)
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_clear
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 4 into %rbx
	MOVQ -32(%rbp), %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# x DIV precision
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r11, %rdx
	# move arg 0 (in %r10) into %rdi
	MOVQ %r10, %rdi
	# move local 5 into %rbx
	MOVQ -40(%rbp), %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# y DIV precision
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r11, %rdx
	# move arg 1 (in %r10) into %rsi
	MOVQ %r10, %rsi

	# move 25 into register
	MOVQ $25, %rbx
	# move arg 2 (in %rbx) into %rdx
	MOVQ %rbx, %rdx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL draw_box
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL gfx_flush
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# move local 8 into %rbx
	MOVQ -64(%rbp), %rbx
	# move local 1 into %r10
	MOVQ -8(%rbp), %r10

	# deltat multiply precision
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	# move arg 0 (in %r10) into %rdi
	MOVQ %r10, %rdi
	# save caller-saved registers
	PUSHQ %r10
	PUSHQ %r11
	# call actual function
	CALL usleep
	# unsave caller-saved registers
	POPQ %r11
	POPQ %r10
	# save function return value
	MOVQ %rax, %rbx
	# unconditional jump to next loop
JMP L0
	# done label
L1:
