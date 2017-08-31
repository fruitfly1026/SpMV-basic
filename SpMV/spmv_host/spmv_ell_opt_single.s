	.file	"spmv_ell.c"
	.text
	.p2align 4,,15
.globl __spmv_ell_serial_host_sse_float
	.type	__spmv_ell_serial_host_sse_float, @function
#void __spmv_ell_serial_host_sse_float(const int num_rows,  #rdi
#                            const int num_cols,  #rsi
#                            const int num_cols_per_row, #rdx
#                            const int stride,    #rcx
#                            const int * Aj,     #r8
#                            const float * Ax,  #r9
#                            const float * x, #stack 
#                                  float * y) #stack
__spmv_ell_serial_host_sse_float:
.LFB2:
	pushq	%r14
.LCFI0:
	movl	%ecx, %r14d
	pushq	%r13
.LCFI1:
	movq	%r8, %r13         #Aj 
	xorl	%r8d, %r8d
	pushq	%r12
.LCFI2:
	movq	%r9, %r12         #Ax
	pushq	%rbp
.LCFI3:
	movl	%edx, %ebp       #num_cols_per_row
	pushq	%rbx
.LCFI4:
	movq	48(%rsp), %r9     #x  
#	xorl	%ebx, %ebx
	movq	56(%rsp), %rcx    #y
	.p2align 4,,7
.L17:
	cmpl	%ebp, %r8d        #middle loop
	jge	.L10
#.L19:
#	addl	$1, %ebx           #outmost 
#	cmpl	$9999, %ebx        #       
#	jg	.L10
#	xorl	%r8d, %r8d
#	cmpl	%ebp, %r8d         #middle loop
#	jge	.L19
.L3:
	movl	%r8d, %eax     #middle index:n
	imull	%r14d, %eax    #stride*n
	cltq
	salq	$2, %rax       #n*stride*4
  ##
  subl  $3, %edi
	testl	%edi, %edi     ##num_rows
	leaq	(%rax,%r13), %rdx     #Aj
	leaq	(%rax,%r12), %r11     #Ax_n
	xorl	%r10d, %r10d
	xorl	%esi, %esi
	jle	.L7
	.p2align 4,,7
######inter most loop ##############
.L6:
  movdqu  (%rdx,%rsi,4), %xmm8  #col_idx:Aj [1-4]
	movups	(%r11,%rsi,4), %xmm0    #value:Ax_n[1-4]
  movd  %xmm8,%eax
  movss (%r9,%rax,4),%xmm1      #x:   [1]

  pextrd  $0b1,%xmm8, %eax
  movss (%r9,%rax,4),%xmm2      #x:   [2]

  pextrd  $0b10,%xmm8,%eax
  movss (%r9,%rax,4),%xmm3      #x:   [3]

  pextrd  $0b11,%xmm8,%eax
  movss (%r9,%rax,4),%xmm4      #x:   [4]

  insertps  $16,%xmm2,%xmm1
  insertps  $32,%xmm3,%xmm1
  insertps  $48,%xmm4,%xmm1

  mulps   %xmm1, %xmm0

	addps	(%rcx,%rsi,4), %xmm0    #y[i]
	movups	%xmm0, (%rcx,%rsi,4)

  addq  $4, %rsi
  addl  $4, %r10d
	cmpl	%edi, %r10d
	jne	.L6
############end loop###############
.L7:
  addl  $3, %edi
  cmpl  %edi,%r10d
  jge   .L4
.L8:
	movslq	(%rdx,%rsi,4),%rax    #Aj
	movss	(%r11,%rsi,4), %xmm0    #Ax_n
	addl	$1, %r10d
	mulss	(%r9,%rax,4), %xmm0     #value*Ax
	addss	(%rcx,%rsi,4), %xmm0    #y[i]
	movss	%xmm0, (%rcx,%rsi,4)
	addq	$1, %rsi
	cmpl	%edi, %r10d
  jl  .L8
  
.L4:
	addl	$1, %r8d                  #middle
	jmp	.L17
.L10:
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
.LFE2:
	.size	__spmv_ell_serial_host_sse_float, .-__spmv_ell_serial_host_sse_float
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x8e
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x8d
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x8c
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x86
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x83
	.uleb128 0x6
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-44)"
	.section	.note.GNU-stack,"",@progbits
