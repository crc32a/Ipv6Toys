	.file	"test_ringcharbuff.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
.globl chop
	.type	chop, @function
chop:
.LFB0:
	.file 1 "test_ringcharbuff.c"
	.loc 1 12 0
	.cfi_startproc
	pushq	%rbp
.LCFI0:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI1:
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 14 0
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movl	%eax, -4(%rbp)
	.loc 1 15 0
	movl	-4(%rbp), %eax
	cltq
	subq	$1, %rax
	addq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$10, %al
	je	.L2
	movl	$0, %eax
	jmp	.L3
.L2:
	.loc 1 16 0
	movl	-4(%rbp), %eax
	cltq
	subq	$1, %rax
	addq	-24(%rbp), %rax
	movb	$0, (%rax)
	.loc 1 17 0
	movl	$1, %eax
.L3:
	.loc 1 18 0
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	chop, .-chop
.globl readline
	.type	readline, @function
readline:
.LFB1:
	.loc 1 20 0
	.cfi_startproc
	pushq	%rbp
.LCFI2:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI3:
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	.loc 1 23 0
	movq	-40(%rbp), %rax
	movl	%eax, %ecx
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	fgets
	movq	%rax, -16(%rbp)
	.loc 1 24 0
	cmpq	$0, -16(%rbp)
	jne	.L6
	.loc 1 25 0
	movl	$0, %eax
	jmp	.L7
.L6:
	.loc 1 27 0
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movl	%eax, -4(%rbp)
	.loc 1 28 0
	movq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	chop
	.loc 1 29 0
	movl	-4(%rbp), %eax
.L7:
	.loc 1 30 0
	leave
	ret
	.cfi_endproc
.LFE1:
	.size	readline, .-readline
	.section	.rodata
.LC0:
	.string	"usage is <%s> ringsize\n"
.LC1:
	.string	"Play with the ringcharbuff"
.LC2:
	.string	"Use commands"
.LC3:
	.string	"   +<STR># to add string STR"
.LC4:
	.string	"   r<n># to read nbytes"
.LC5:
	.string	"   -<n># to remove nbytes"
.LC6:
	.string	"   s<n># to resize ringbuff"
.LC7:
	.string	"   l<STR> add STR as a line"
	.text
.globl usage
	.type	usage, @function
usage:
.LFB2:
	.loc 1 32 0
	.cfi_startproc
	pushq	%rbp
.LCFI4:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI5:
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	.loc 1 33 0
	movl	$.LC0, %eax
	movq	-8(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 34 0
	movl	$10, %edi
	call	putchar
	.loc 1 35 0
	movl	$.LC1, %edi
	call	puts
	.loc 1 36 0
	movl	$.LC2, %edi
	call	puts
	.loc 1 37 0
	movl	$.LC3, %edi
	call	puts
	.loc 1 38 0
	movl	$.LC4, %edi
	call	puts
	.loc 1 39 0
	movl	$.LC5, %edi
	call	puts
	.loc 1 40 0
	movl	$.LC6, %edi
	call	puts
	.loc 1 41 0
	movl	$.LC7, %edi
	call	puts
	.loc 1 42 0
	movl	$0, %eax
	.loc 1 43 0
	leave
	ret
	.cfi_endproc
.LFE2:
	.size	usage, .-usage
	.section	.rodata
.LC8:
	.string	"mypid = %i\n"
.LC9:
	.string	"rl resp= %i\n"
.LC10:
	.string	"\n"
.LC11:
	.string	"Wrote %i bytes\n"
.LC12:
	.string	"removed %i bytes\n"
.LC13:
	.string	"Read %i bytes \"%s\"\n"
.LC14:
	.string	"%i = hasline(r)\n"
.LC15:
	.string	"pid=%i\n"
.LC16:
	.string	"%i\n"
.LC17:
	.string	"Error in resize %i\n"
.LC18:
	.string	"Resize success"
.LC19:
	.string	"Used=%i\n"
.LC20:
	.string	"free=%i\n"
.LC21:
	.string	"size=%i\n"
.LC22:
	.string	"Unknown Symbol use Q to quite"
	.text
.globl main
	.type	main, @function
main:
.LFB3:
	.loc 1 46 0
	.cfi_startproc
	pushq	%rbp
.LCFI6:
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
.LCFI7:
	.cfi_def_cfa_register 6
	subq	$3184, %rsp
	movl	%edi, -3172(%rbp)
	movq	%rsi, -3184(%rbp)
	.loc 1 46 0
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	.loc 1 60 0
	cmpl	$1, -3172(%rbp)
	jg	.L12
	.loc 1 61 0
	movq	-3184(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	usage
	.loc 1 62 0
	movl	$0, %eax
	jmp	.L11
.L12:
	.loc 1 65 0
	movq	-3184(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -3124(%rbp)
	.loc 1 67 0
	call	getpid
	movl	%eax, %edx
	movl	$.LC8, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 68 0
	movl	-3124(%rbp), %eax
	cltq
	leaq	-3128(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_init
	movq	%rax, -3160(%rbp)
	.loc 1 69 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_print
.L31:
	.loc 1 71 0
	movl	$62, %edi
	call	putchar
	.loc 1 72 0
	movq	stdout(%rip), %rax
	movq	%rax, %rdi
	call	fflush
	.loc 1 73 0
	movq	stdin(%rip), %rax
	leaq	-2080(%rbp), %rcx
	movl	$1024, %edx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	readline
	movl	%eax, -3144(%rbp)
	.loc 1 74 0
	cmpl	$0, -3144(%rbp)
	jg	.L13
	.loc 1 75 0
	movl	$.LC9, %eax
	movl	-3144(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	jmp	.L11
.L13:
	.loc 1 78 0
	movzbl	-2080(%rbp), %eax
	movsbl	%al,%eax
	subl	$43, %eax
	cmpl	$74, %eax
	ja	.L14
	mov	%eax, %eax
	movq	.L27(,%rax,8), %rax
	jmp	*%rax
	.section	.rodata
	.align 8
	.align 4
.L27:
	.quad	.L15
	.quad	.L14
	.quad	.L16
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L17
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L18
	.quad	.L19
	.quad	.L20
	.quad	.L21
	.quad	.L14
	.quad	.L14
	.quad	.L22
	.quad	.L14
	.quad	.L14
	.quad	.L14
	.quad	.L23
	.quad	.L17
	.quad	.L24
	.quad	.L25
	.quad	.L14
	.quad	.L26
	.text
.L22:
	.loc 1 79 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 80 0
	movl	$.LC10, %edx
	movq	-3168(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcat
	.loc 1 81 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movl	%eax, -3136(%rbp)
	.loc 1 82 0
	movl	-3136(%rbp), %eax
	movslq	%eax,%rdx
	movq	-3168(%rbp), %rcx
	movq	-3160(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_add
	movl	%eax, -3144(%rbp)
	.loc 1 83 0
	movl	$.LC11, %eax
	movl	-3144(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 84 0
	jmp	.L28
.L15:
	.loc 1 86 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 87 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movl	%eax, -3136(%rbp)
	.loc 1 88 0
	movl	-3136(%rbp), %eax
	movslq	%eax,%rdx
	movq	-3168(%rbp), %rcx
	movq	-3160(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_add
	movl	%eax, -3144(%rbp)
	.loc 1 89 0
	movl	$.LC11, %eax
	movl	-3144(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 90 0
	jmp	.L28
.L16:
	.loc 1 92 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 93 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -3136(%rbp)
	.loc 1 94 0
	movl	-3136(%rbp), %eax
	movslq	%eax,%rdx
	movq	-3160(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_dec
	movl	%eax, -3144(%rbp)
	.loc 1 95 0
	movl	$.LC12, %eax
	movl	-3144(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 96 0
	jmp	.L28
.L19:
	.loc 1 98 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 99 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -3136(%rbp)
	.loc 1 100 0
	movl	-3136(%rbp), %eax
	movslq	%eax,%rdx
	leaq	-3120(%rbp), %rcx
	movq	-3160(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_get
	movl	%eax, -3144(%rbp)
	.loc 1 101 0
	movl	-3144(%rbp), %eax
	cltq
	movb	$0, -3120(%rbp,%rax)
	.loc 1 102 0
	movl	$.LC13, %eax
	leaq	-3120(%rbp), %rdx
	movl	-3144(%rbp), %ecx
	movl	%ecx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 103 0
	jmp	.L28
.L20:
	.loc 1 105 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 106 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -3136(%rbp)
	.loc 1 107 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_hasline
	movl	%eax, -3144(%rbp)
	.loc 1 108 0
	movl	$.LC14, %eax
	movl	-3144(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 109 0
	jmp	.L28
.L21:
	.loc 1 111 0
	call	getpid
	movl	%eax, %edx
	movl	$.LC15, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 112 0
	jmp	.L28
.L23:
	.loc 1 114 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_print
	.loc 1 115 0
	jmp	.L28
.L24:
	.loc 1 117 0
	leaq	-2080(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -3168(%rbp)
	.loc 1 118 0
	movq	-3168(%rbp), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -3136(%rbp)
	.loc 1 119 0
	movl	$.LC16, %eax
	movl	-3136(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 120 0
	movl	-3136(%rbp), %eax
	movslq	%eax,%rcx
	leaq	-3128(%rbp), %rdx
	movq	-3160(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	ringcharbuff_resize
	movl	%eax, -3144(%rbp)
	.loc 1 121 0
	cmpl	$0, -3144(%rbp)
	jne	.L29
	.loc 1 122 0
	movl	-3128(%rbp), %edx
	movl	$.LC17, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	jmp	.L30
.L29:
	.loc 1 124 0
	movl	$.LC18, %edi
	call	puts
.L30:
	.loc 1 126 0
	jmp	.L28
.L26:
	.loc 1 128 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_used
	movl	%eax, %edx
	movl	$.LC19, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 129 0
	jmp	.L28
.L18:
	.loc 1 131 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_free
	movl	%eax, %edx
	movl	$.LC20, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 132 0
	jmp	.L28
.L25:
	.loc 1 134 0
	movq	-3160(%rbp), %rax
	movq	%rax, %rdi
	call	ringcharbuff_size
	movl	%eax, %edx
	movl	$.LC21, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	.loc 1 135 0
	jmp	.L28
.L17:
	.loc 1 137 0
	movl	$0, %edi
	call	exit
.L14:
	.loc 1 140 0
	movl	$.LC22, %edi
	call	puts
	.loc 1 143 0
	jmp	.L31
.L28:
	jmp	.L31
.L11:
	.loc 1 144 0
	movq	-8(%rbp), %rdx
	xorq	%fs:40, %rdx
	je	.L33
	call	__stack_chk_fail
.L33:
	leave
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.quad	.LFB0-.Ltext0
	.quad	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI0-.Ltext0
	.quad	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI1-.Ltext0
	.quad	.LFE0-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST1:
	.quad	.LFB1-.Ltext0
	.quad	.LCFI2-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI2-.Ltext0
	.quad	.LCFI3-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI3-.Ltext0
	.quad	.LFE1-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST2:
	.quad	.LFB2-.Ltext0
	.quad	.LCFI4-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI4-.Ltext0
	.quad	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI5-.Ltext0
	.quad	.LFE2-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
.LLST3:
	.quad	.LFB3-.Ltext0
	.quad	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 8
	.quad	.LCFI6-.Ltext0
	.quad	.LCFI7-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 16
	.quad	.LCFI7-.Ltext0
	.quad	.LFE3-.Ltext0
	.value	0x2
	.byte	0x76
	.sleb128 16
	.quad	0x0
	.quad	0x0
	.file 2 "/usr/lib/gcc/x86_64-linux-gnu/4.4.3/include/stddef.h"
	.file 3 "/usr/include/bits/types.h"
	.file 4 "/usr/include/stdio.h"
	.file 5 "/usr/include/libio.h"
	.file 6 "ringcharbuff.h"
	.section	.debug_info
	.long	0x546
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF71
	.byte	0x1
	.long	.LASF72
	.long	.LASF73
	.quad	.Ltext0
	.quad	.Letext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF8
	.byte	0x2
	.byte	0xd3
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF1
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF2
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF3
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF5
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF6
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF7
	.uleb128 0x2
	.long	.LASF9
	.byte	0x3
	.byte	0x8d
	.long	0x46
	.uleb128 0x2
	.long	.LASF10
	.byte	0x3
	.byte	0x8e
	.long	0x46
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.uleb128 0x6
	.byte	0x8
	.uleb128 0x7
	.byte	0x8
	.long	0x98
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF11
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF12
	.uleb128 0x2
	.long	.LASF13
	.byte	0x4
	.byte	0x31
	.long	0xb1
	.uleb128 0x8
	.long	.LASF43
	.byte	0xd8
	.byte	0x4
	.byte	0x2d
	.long	0x27d
	.uleb128 0x9
	.long	.LASF14
	.byte	0x5
	.value	0x110
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x9
	.long	.LASF15
	.byte	0x5
	.value	0x115
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x9
	.long	.LASF16
	.byte	0x5
	.value	0x116
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x9
	.long	.LASF17
	.byte	0x5
	.value	0x117
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x9
	.long	.LASF18
	.byte	0x5
	.value	0x118
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x9
	.long	.LASF19
	.byte	0x5
	.value	0x119
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x28
	.uleb128 0x9
	.long	.LASF20
	.byte	0x5
	.value	0x11a
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x9
	.long	.LASF21
	.byte	0x5
	.value	0x11b
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x9
	.long	.LASF22
	.byte	0x5
	.value	0x11c
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0x9
	.long	.LASF23
	.byte	0x5
	.value	0x11e
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0x9
	.long	.LASF24
	.byte	0x5
	.value	0x11f
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x50
	.uleb128 0x9
	.long	.LASF25
	.byte	0x5
	.value	0x120
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x58
	.uleb128 0x9
	.long	.LASF26
	.byte	0x5
	.value	0x122
	.long	0x2bb
	.byte	0x2
	.byte	0x23
	.uleb128 0x60
	.uleb128 0x9
	.long	.LASF27
	.byte	0x5
	.value	0x124
	.long	0x2c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x68
	.uleb128 0x9
	.long	.LASF28
	.byte	0x5
	.value	0x126
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x70
	.uleb128 0x9
	.long	.LASF29
	.byte	0x5
	.value	0x12a
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x74
	.uleb128 0x9
	.long	.LASF30
	.byte	0x5
	.value	0x12c
	.long	0x77
	.byte	0x2
	.byte	0x23
	.uleb128 0x78
	.uleb128 0x9
	.long	.LASF31
	.byte	0x5
	.value	0x130
	.long	0x5b
	.byte	0x3
	.byte	0x23
	.uleb128 0x80
	.uleb128 0x9
	.long	.LASF32
	.byte	0x5
	.value	0x131
	.long	0x69
	.byte	0x3
	.byte	0x23
	.uleb128 0x82
	.uleb128 0x9
	.long	.LASF33
	.byte	0x5
	.value	0x132
	.long	0x2c7
	.byte	0x3
	.byte	0x23
	.uleb128 0x83
	.uleb128 0x9
	.long	.LASF34
	.byte	0x5
	.value	0x136
	.long	0x2d7
	.byte	0x3
	.byte	0x23
	.uleb128 0x88
	.uleb128 0x9
	.long	.LASF35
	.byte	0x5
	.value	0x13f
	.long	0x82
	.byte	0x3
	.byte	0x23
	.uleb128 0x90
	.uleb128 0x9
	.long	.LASF36
	.byte	0x5
	.value	0x148
	.long	0x90
	.byte	0x3
	.byte	0x23
	.uleb128 0x98
	.uleb128 0x9
	.long	.LASF37
	.byte	0x5
	.value	0x149
	.long	0x90
	.byte	0x3
	.byte	0x23
	.uleb128 0xa0
	.uleb128 0x9
	.long	.LASF38
	.byte	0x5
	.value	0x14a
	.long	0x90
	.byte	0x3
	.byte	0x23
	.uleb128 0xa8
	.uleb128 0x9
	.long	.LASF39
	.byte	0x5
	.value	0x14b
	.long	0x90
	.byte	0x3
	.byte	0x23
	.uleb128 0xb0
	.uleb128 0x9
	.long	.LASF40
	.byte	0x5
	.value	0x14c
	.long	0x2d
	.byte	0x3
	.byte	0x23
	.uleb128 0xb8
	.uleb128 0x9
	.long	.LASF41
	.byte	0x5
	.value	0x14e
	.long	0x3f
	.byte	0x3
	.byte	0x23
	.uleb128 0xc0
	.uleb128 0x9
	.long	.LASF42
	.byte	0x5
	.value	0x150
	.long	0x2dd
	.byte	0x3
	.byte	0x23
	.uleb128 0xc4
	.byte	0x0
	.uleb128 0xa
	.long	.LASF74
	.byte	0x5
	.byte	0xb4
	.uleb128 0x8
	.long	.LASF44
	.byte	0x18
	.byte	0x5
	.byte	0xba
	.long	0x2bb
	.uleb128 0xb
	.long	.LASF45
	.byte	0x5
	.byte	0xbb
	.long	0x2bb
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xb
	.long	.LASF46
	.byte	0x5
	.byte	0xbc
	.long	0x2c1
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0xb
	.long	.LASF47
	.byte	0x5
	.byte	0xc0
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x284
	.uleb128 0x7
	.byte	0x8
	.long	0xb1
	.uleb128 0xc
	.long	0x98
	.long	0x2d7
	.uleb128 0xd
	.long	0x8d
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x27d
	.uleb128 0xc
	.long	0x98
	.long	0x2ed
	.uleb128 0xd
	.long	0x8d
	.byte	0x13
	.byte	0x0
	.uleb128 0xe
	.byte	0x18
	.byte	0x6
	.byte	0x3
	.long	0x32c
	.uleb128 0xf
	.string	"h"
	.byte	0x6
	.byte	0x4
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xb
	.long	.LASF48
	.byte	0x6
	.byte	0x5
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0xb
	.long	.LASF49
	.byte	0x6
	.byte	0x6
	.long	0x3f
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0xb
	.long	.LASF50
	.byte	0x6
	.byte	0x7
	.long	0x92
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.byte	0x0
	.uleb128 0x2
	.long	.LASF51
	.byte	0x6
	.byte	0x8
	.long	0x2ed
	.uleb128 0x10
	.byte	0x1
	.long	.LASF52
	.byte	0x1
	.byte	0xc
	.byte	0x1
	.long	0x3f
	.quad	.LFB0
	.quad	.LFE0
	.long	.LLST0
	.long	0x379
	.uleb128 0x11
	.long	.LASF54
	.byte	0x1
	.byte	0xc
	.long	0x92
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x12
	.long	.LASF57
	.byte	0x1
	.byte	0xd
	.long	0x3f
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF53
	.byte	0x1
	.byte	0x14
	.byte	0x1
	.long	0x3f
	.quad	.LFB1
	.quad	.LFE1
	.long	.LLST1
	.long	0x3e4
	.uleb128 0x13
	.string	"fp"
	.byte	0x1
	.byte	0x14
	.long	0x3e4
	.byte	0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x11
	.long	.LASF55
	.byte	0x1
	.byte	0x14
	.long	0x92
	.byte	0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x11
	.long	.LASF56
	.byte	0x1
	.byte	0x14
	.long	0x2d
	.byte	0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0x14
	.string	"out"
	.byte	0x1
	.byte	0x15
	.long	0x3f
	.byte	0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x14
	.string	"res"
	.byte	0x1
	.byte	0x16
	.long	0x92
	.byte	0x2
	.byte	0x91
	.sleb128 -32
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0xa6
	.uleb128 0x10
	.byte	0x1
	.long	.LASF58
	.byte	0x1
	.byte	0x20
	.byte	0x1
	.long	0x3f
	.quad	.LFB2
	.quad	.LFE2
	.long	.LLST2
	.long	0x41e
	.uleb128 0x11
	.long	.LASF59
	.byte	0x1
	.byte	0x20
	.long	0x92
	.byte	0x2
	.byte	0x91
	.sleb128 -24
	.byte	0x0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF60
	.byte	0x1
	.byte	0x2e
	.byte	0x1
	.long	0x3f
	.quad	.LFB3
	.quad	.LFE3
	.long	.LLST3
	.long	0x512
	.uleb128 0x11
	.long	.LASF61
	.byte	0x1
	.byte	0x2e
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3188
	.uleb128 0x11
	.long	.LASF62
	.byte	0x1
	.byte	0x2e
	.long	0x512
	.byte	0x3
	.byte	0x91
	.sleb128 -3200
	.uleb128 0x14
	.string	"fpi"
	.byte	0x1
	.byte	0x2f
	.long	0x3e4
	.byte	0x3
	.byte	0x91
	.sleb128 -3168
	.uleb128 0x14
	.string	"r"
	.byte	0x1
	.byte	0x30
	.long	0x518
	.byte	0x3
	.byte	0x91
	.sleb128 -3176
	.uleb128 0x12
	.long	.LASF63
	.byte	0x1
	.byte	0x31
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3140
	.uleb128 0x14
	.string	"err"
	.byte	0x1
	.byte	0x32
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3144
	.uleb128 0x14
	.string	"w"
	.byte	0x1
	.byte	0x33
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3148
	.uleb128 0x12
	.long	.LASF64
	.byte	0x1
	.byte	0x34
	.long	0x51e
	.byte	0x3
	.byte	0x91
	.sleb128 -1056
	.uleb128 0x12
	.long	.LASF65
	.byte	0x1
	.byte	0x35
	.long	0x51e
	.byte	0x3
	.byte	0x91
	.sleb128 -2096
	.uleb128 0x12
	.long	.LASF50
	.byte	0x1
	.byte	0x36
	.long	0x51e
	.byte	0x3
	.byte	0x91
	.sleb128 -3136
	.uleb128 0x12
	.long	.LASF66
	.byte	0x1
	.byte	0x37
	.long	0x92
	.byte	0x3
	.byte	0x91
	.sleb128 -3184
	.uleb128 0x14
	.string	"len"
	.byte	0x1
	.byte	0x38
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3152
	.uleb128 0x12
	.long	.LASF67
	.byte	0x1
	.byte	0x39
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3156
	.uleb128 0x12
	.long	.LASF68
	.byte	0x1
	.byte	0x3a
	.long	0x3f
	.byte	0x3
	.byte	0x91
	.sleb128 -3160
	.byte	0x0
	.uleb128 0x7
	.byte	0x8
	.long	0x92
	.uleb128 0x7
	.byte	0x8
	.long	0x32c
	.uleb128 0xc
	.long	0x98
	.long	0x52f
	.uleb128 0x15
	.long	0x8d
	.value	0x400
	.byte	0x0
	.uleb128 0x16
	.long	.LASF69
	.byte	0x4
	.byte	0x91
	.long	0x2c1
	.byte	0x1
	.byte	0x1
	.uleb128 0x16
	.long	.LASF70
	.byte	0x4
	.byte	0x92
	.long	0x2c1
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x37
	.value	0x2
	.long	.Ldebug_info0
	.long	0x54a
	.long	0x337
	.string	"chop"
	.long	0x379
	.string	"readline"
	.long	0x3ea
	.string	"usage"
	.long	0x41e
	.string	"main"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0x0
	.value	0x0
	.value	0x0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0x0
	.quad	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF30:
	.string	"_old_offset"
.LASF17:
	.string	"_IO_read_base"
.LASF58:
	.string	"usage"
.LASF25:
	.string	"_IO_save_end"
.LASF64:
	.string	"tmp_line"
.LASF7:
	.string	"short int"
.LASF8:
	.string	"size_t"
.LASF35:
	.string	"_offset"
.LASF19:
	.string	"_IO_write_ptr"
.LASF14:
	.string	"_flags"
.LASF53:
	.string	"readline"
.LASF68:
	.string	"resp"
.LASF21:
	.string	"_IO_buf_base"
.LASF26:
	.string	"_markers"
.LASF16:
	.string	"_IO_read_end"
.LASF54:
	.string	"str_in"
.LASF65:
	.string	"line"
.LASF31:
	.string	"_cur_column"
.LASF2:
	.string	"long long int"
.LASF34:
	.string	"_lock"
.LASF52:
	.string	"chop"
.LASF55:
	.string	"line_out"
.LASF67:
	.string	"is_eof"
.LASF47:
	.string	"_pos"
.LASF62:
	.string	"argv"
.LASF57:
	.string	"length"
.LASF3:
	.string	"unsigned char"
.LASF61:
	.string	"argc"
.LASF6:
	.string	"signed char"
.LASF12:
	.string	"long long unsigned int"
.LASF59:
	.string	"prog"
.LASF5:
	.string	"unsigned int"
.LASF44:
	.string	"_IO_marker"
.LASF33:
	.string	"_shortbuf"
.LASF18:
	.string	"_IO_write_base"
.LASF42:
	.string	"_unused2"
.LASF15:
	.string	"_IO_read_ptr"
.LASF40:
	.string	"__pad5"
.LASF22:
	.string	"_IO_buf_end"
.LASF11:
	.string	"char"
.LASF43:
	.string	"_IO_FILE"
.LASF1:
	.string	"long int"
.LASF60:
	.string	"main"
.LASF45:
	.string	"_next"
.LASF36:
	.string	"__pad1"
.LASF37:
	.string	"__pad2"
.LASF38:
	.string	"__pad3"
.LASF39:
	.string	"__pad4"
.LASF73:
	.string	"/home/crc/workspace/Ipv6Toys/tunnel_server"
.LASF50:
	.string	"data"
.LASF4:
	.string	"short unsigned int"
.LASF0:
	.string	"long unsigned int"
.LASF20:
	.string	"_IO_write_end"
.LASF10:
	.string	"__off64_t"
.LASF56:
	.string	"nbytes"
.LASF28:
	.string	"_fileno"
.LASF27:
	.string	"_chain"
.LASF48:
	.string	"size"
.LASF9:
	.string	"__off_t"
.LASF63:
	.string	"ringsize"
.LASF51:
	.string	"ringcharbuff_t"
.LASF24:
	.string	"_IO_backup_base"
.LASF69:
	.string	"stdin"
.LASF29:
	.string	"_flags2"
.LASF41:
	.string	"_mode"
.LASF49:
	.string	"used"
.LASF72:
	.string	"test_ringcharbuff.c"
.LASF32:
	.string	"_vtable_offset"
.LASF23:
	.string	"_IO_save_base"
.LASF71:
	.string	"GNU C 4.4.3"
.LASF13:
	.string	"FILE"
.LASF46:
	.string	"_sbuf"
.LASF70:
	.string	"stdout"
.LASF74:
	.string	"_IO_lock_t"
.LASF66:
	.string	"l_ptr"
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
	.section	.note.GNU-stack,"",@progbits
