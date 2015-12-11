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
	PUSHQ %rbp
	MOVQ %rsp, %rbp
	PUSHQ %rdi
	PUSHQ %rsi
	PUSHQ %rdx
	SUBQ $0, %rsp
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15
	MOVQ $255, %rbx
	MOVQ %rbx, %rdi
	MOVQ $255, %rbx
	MOVQ %rbx, %rsi
	MOVQ $255, %rbx
	MOVQ %rbx, %rdx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_color
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdi
	MOVQ -16(%rbp), %rbx
	MOVQ %rbx, %rsi
	MOVQ -8(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rdx
	MOVQ -16(%rbp), %rbx
	MOVQ %rbx, %rcx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_line
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rdi
	MOVQ -16(%rbp), %rbx
	MOVQ %rbx, %rsi
	MOVQ -8(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rdx
	MOVQ -16(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rcx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_line
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rdi
	MOVQ -16(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rsi
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdx
	MOVQ -16(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rcx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_line
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdi
	MOVQ -16(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rsi
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdx
	MOVQ -16(%rbp), %rbx
	MOVQ %rbx, %rcx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_line
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.globl main
main:
	PUSHQ %rbp
	MOVQ %rsp, %rbp
	SUBQ $72, %rsp
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15
	MOVQ $100, %rbx
	MOVQ %rbx, -8(%rbp)
	MOVQ $500, %rbx
	MOVQ %rbx, -16(%rbp)
	MOVQ $500, %rbx
	MOVQ %rbx, -24(%rbp)
	MOVQ -8(%rbp), %rbx
	MOVQ -16(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ $2, %rbx
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -32(%rbp)
	MOVQ -8(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ $2, %rbx
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -40(%rbp)
	MOVQ -8(%rbp), %rbx
	MOVQ $3, %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -48(%rbp)
	MOVQ -8(%rbp), %rbx
	MOVQ $5, %r10
	NEG %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -56(%rbp)
	MOVQ $100, %rbx
	MOVQ %rbx, -64(%rbp)
	MOVQ -16(%rbp), %rbx
	MOVQ %rbx, %rdi
	MOVQ -24(%rbp), %rbx
	MOVQ %rbx, %rsi
.data
STR0:
	.string "Bounce!"
.text
	LEAQ STR0, %rbx
	MOVQ %rbx, %rdx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_open
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
L0:
.data
STR1:
	.string "x: "
.text
	LEAQ STR1, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -32(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR2:
	.string " y: "
.text
	LEAQ STR2, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -40(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR3:
	.string " vx: "
.text
	LEAQ STR3, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -48(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR4:
	.string " vy: "
.text
	LEAQ STR4, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -56(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR5:
	.string "\n"
.text
	LEAQ STR5, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_event_waiting
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	CMP $0, %rbx
	JE L2
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_wait
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ %rbx, -72(%rbp)
	MOVQ -72(%rbp), %rbx
	MOVQ $113, %r10
	CMP %r10, %rbx
	JE L4
	MOVQ $0, %r10
	JMP L5
L4:
	MOVQ $1, %r10
L5:
	CMP $0, %r10
	JE L3
	MOVQ $0, %rbx
	MOVQ %rbx, %rax
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	MOVQ %rbp, %rsp
	POPQ %rbp
	RET
L3:
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_xpos
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -32(%rbp)
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_ypos
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -40(%rbp)
	MOVQ $5, %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, -48(%rbp)
L2:
	MOVQ -56(%rbp), %rbx
	MOVQ $1, %r10
	MOVQ -8(%rbp), %r11
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	ADDQ %r11, %rbx
	MOVQ %rbx, -56(%rbp)
	MOVQ -32(%rbp), %rbx
	MOVQ $0, %r10
	CMP %r10, %rbx
	JL L7
	MOVQ $0, %r10
	JMP L8
L7:
	MOVQ $1, %r10
L8:
	MOVQ -48(%rbp), %rbx
	MOVQ $0, %r11
	CMP %r11, %rbx
	JL L9
	MOVQ $0, %r11
	JMP L10
L9:
	MOVQ $1, %r11
L10:
	CMP $0, %r10
	JE L12
	CMP $0, %r11
	JNE L11
L12:
	MOVQ $0, %r11
	JMP L13
L11:
	MOVQ $1, %r11
L13:
	CMP $0, %r11
	JE L6
	MOVQ $9, %rbx
	NEG %rbx
	MOVQ -48(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ $10, %rbx
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -48(%rbp)
L6:
	MOVQ -32(%rbp), %rbx
	MOVQ -16(%rbp), %r10
	MOVQ -8(%rbp), %r11
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	CMP %r11, %rbx
	JG L15
	MOVQ $0, %r11
	JMP L16
L15:
	MOVQ $1, %r11
L16:
	MOVQ -48(%rbp), %rbx
	MOVQ $0, %r10
	CMP %r10, %rbx
	JG L17
	MOVQ $0, %r10
	JMP L18
L17:
	MOVQ $1, %r10
L18:
	CMP $0, %r11
	JE L20
	CMP $0, %r10
	JNE L19
L20:
	MOVQ $0, %r10
	JMP L21
L19:
	MOVQ $1, %r10
L21:
	CMP $0, %r10
	JE L14
	MOVQ $9, %rbx
	NEG %rbx
	MOVQ -48(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ $10, %rbx
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -48(%rbp)
L14:
	MOVQ -40(%rbp), %rbx
	MOVQ -24(%rbp), %r10
	MOVQ -8(%rbp), %r11
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	CMP %r11, %rbx
	JG L23
	MOVQ $0, %r11
	JMP L24
L23:
	MOVQ $1, %r11
L24:
	MOVQ -56(%rbp), %rbx
	MOVQ $0, %r10
	CMP %r10, %rbx
	JG L25
	MOVQ $0, %r10
	JMP L26
L25:
	MOVQ $1, %r10
L26:
	CMP $0, %r11
	JE L28
	CMP $0, %r10
	JNE L27
L28:
	MOVQ $0, %r10
	JMP L29
L27:
	MOVQ $1, %r10
L29:
	CMP $0, %r10
	JE L22
	MOVQ $9, %rbx
	NEG %rbx
	MOVQ -56(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ $10, %rbx
	MOVQ %rdx, %r11
	MOVQ %r10, %rax
	CQO
	IDIVQ %rbx
	MOVQ %rax, %rbx
	MOVQ %r11, %rdx
	MOVQ %rbx, -56(%rbp)
L22:
	MOVQ -32(%rbp), %rbx
	MOVQ -48(%rbp), %r10
	MOVQ -64(%rbp), %r11
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	MOVQ -8(%rbp), %r10
	MOVQ %rdx, %r12
	MOVQ %r11, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r12, %rdx
	ADDQ %r10, %rbx
	MOVQ %rbx, -32(%rbp)
	MOVQ -40(%rbp), %rbx
	MOVQ -56(%rbp), %r10
	MOVQ -64(%rbp), %r11
	MOVQ %r10, %rax
	IMULQ %r11
	MOVQ %rax, %r11
	MOVQ -8(%rbp), %r10
	MOVQ %rdx, %r12
	MOVQ %r11, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r12, %rdx
	ADDQ %r10, %rbx
	MOVQ %rbx, -40(%rbp)
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_clear
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -32(%rbp), %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r11, %rdx
	MOVQ %r10, %rdi
	MOVQ -40(%rbp), %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rax, %r10
	MOVQ %r11, %rdx
	MOVQ %r10, %rsi
	MOVQ $25, %rbx
	MOVQ %rbx, %rdx
	PUSHQ %r10
	PUSHQ %r11
	CALL draw_box
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	PUSHQ %r10
	PUSHQ %r11
	CALL gfx_flush
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -64(%rbp), %rbx
	MOVQ -8(%rbp), %r10
	MOVQ %rbx, %rax
	IMULQ %r10
	MOVQ %rax, %r10
	MOVQ %r10, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL usleep
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
JMP L0
L1:
