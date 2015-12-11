.text
.globl fib
fib:
	PUSHQ %rbp
	MOVQ %rsp, %rbp
	PUSHQ %rdi
	SUBQ $0, %rsp
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15
	MOVQ -8(%rbp), %rbx
	MOVQ $1, %r10
	CMP %r10, %rbx
	JL L1
	MOVQ $0, %r10
	JMP L2
L1:
	MOVQ $1, %r10
L2:
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
JMP L0
L3:
	MOVQ -8(%rbp), %rbx
	MOVQ $2, %r10
	CMP %r10, %rbx
	JL L5
	MOVQ $0, %r10
	JMP L6
L5:
	MOVQ $1, %r10
L6:
	CMP $0, %r10
	JE L7
	MOVQ $1, %rbx
	MOVQ %rbx, %rax
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	MOVQ %rbp, %rsp
	POPQ %rbp
	RET
JMP L4
L7:
	MOVQ -8(%rbp), %rbx
	MOVQ $1, %r10
	SUBQ %r10, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL fib
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %r10
	MOVQ $2, %r11
	SUBQ %r11, %r10
	MOVQ %r10, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL fib
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, %rax
	POPQ %r15
	POPQ %r14
	POPQ %r13
	POPQ %r12
	POPQ %rbx
	MOVQ %rbp, %rsp
	POPQ %rbp
	RET
L4:
L0:
.globl main
main:
	PUSHQ %rbp
	MOVQ %rsp, %rbp
	PUSHQ %rdi
	PUSHQ %rsi
	SUBQ $8, %rsp
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15
.data
STR0:
	.string "Table of Fibonnacci Numbers\n"
.text
	LEAQ STR0, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ $0, %rbx
	MOVQ %rbx, -8(%rbp)
L8:
	MOVQ -8(%rbp), %rbx
	MOVQ $45, %r10
	CMP %r10, %rbx
	JL L10
	MOVQ $0, %r10
	JMP L11
L10:
	MOVQ $1, %r10
L11:
	CMP $0, %r10
	JE L9
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR1:
	.string "t"
.text
	LEAQ STR1, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL fib
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_integer
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
.data
STR2:
	.string "\n"
.text
	LEAQ STR2, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -8(%rbp), %rbx
	MOVQ $1, %r10
	ADDQ %r10, %rbx
	MOVQ %rbx, -8(%rbp)
JMP L8
L9:
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
