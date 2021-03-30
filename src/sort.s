	.file	"sort.c"
	.text
	.globl	sort
	.type	sort, @function
sort:
.LFB0:
	.cfi_startproc
	endbr64
	testl	%esi, %esi
	jle	.L1
	movq	%rdi, %r8
	leal	-1(%rsi), %r10d
	addq	$1, %r10
	movl	$1, %r9d
	jmp	.L5
.L3:
	addq	$1, %rax
	cmpl	%eax, %esi
	jle	.L7
.L4:
	movl	(%r8), %edx
	movl	(%rdi,%rax,4), %ecx
	cmpl	%ecx, %edx
	jle	.L3
	movl	%ecx, (%r8)
	movl	%edx, (%rdi,%rax,4)
	jmp	.L3
.L7:
	addq	$1, %r9
	addq	$4, %r8
.L5:
	cmpq	%r10, %r9
	je	.L1
	movq	%r9, %rax
	jmp	.L4
.L1:
	ret
	.cfi_endproc
.LFE0:
	.size	sort, .-sort
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
