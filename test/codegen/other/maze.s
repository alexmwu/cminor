.text
.globl rand
.globl srand
.globl time
.globl main
main:
	PUSHQ %rbp
	MOVQ %rsp, %rbp
	SUBQ $32, %rsp
	PUSHQ %rbx
	PUSHQ %r12
	PUSHQ %r13
	PUSHQ %r14
	PUSHQ %r15
	MOVQ $20, %rbx
	MOVQ %rbx, -8(%rbp)
	MOVQ $20, %rbx
	MOVQ %rbx, -16(%rbp)
	MOVQ $0, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL time
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL srand
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ $0, %rbx
	MOVQ %rbx, -32(%rbp)
L0:
	MOVQ -32(%rbp), %rbx
	MOVQ -16(%rbp), %r10
	CMP %r10, %rbx
	JL L2
	MOVQ $0, %r10
	JMP L3
L2:
	MOVQ $1, %r10
L3:
	CMP $0, %r10
	JE L1
	MOVQ $0, %rbx
	MOVQ %rbx, -24(%rbp)
L4:
	MOVQ -24(%rbp), %rbx
	MOVQ -8(%rbp), %r10
	CMP %r10, %rbx
	JL L6
	MOVQ $0, %r10
	JMP L7
L6:
	MOVQ $1, %r10
L7:
	CMP $0, %r10
	JE L5
	PUSHQ %r10
	PUSHQ %r11
	CALL rand
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ $2, %r10
	MOVQ %rdx, %r11
	MOVQ %rbx, %rax
	CQO
	IDIVQ %r10
	MOVQ %rdx, %r10
	MOVQ %r11, %rdx
	MOVQ $1, %rbx
	CMP %rbx, %r10
	JE L9
	MOVQ $0, %rbx
	JMP L10
L9:
	MOVQ $1, %rbx
L10:
	CMP $0, %rbx
	JE L11
	MOVQ $47, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_character
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
JMP L8
L11:
	MOVQ $92, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_character
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
L8:
	MOVQ -24(%rbp), %rax
	MOVQ %rax, %rbx
	INCQ %rax
	MOVQ %rax, -24(%rbp)
JMP L4
L5:
.data
STR0:
	.string "\n"
.text
	LEAQ STR0, %rbx
	MOVQ %rbx, %rdi
	PUSHQ %r10
	PUSHQ %r11
	CALL print_string
	POPQ %r11
	POPQ %r10
	MOVQ %rax, %rbx
	MOVQ -32(%rbp), %rax
	MOVQ %rax, %rbx
	INCQ %rax
	MOVQ %rax, -32(%rbp)
JMP L0
L1:
