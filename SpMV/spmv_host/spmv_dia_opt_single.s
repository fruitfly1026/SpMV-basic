	.file	"spmv_dia.c"
	.text
	.p2align 4,,15
.globl __spmv_dia_serial_host_sse_float
	.type	__spmv_dia_serial_host_sse_float, @function
#void __spmv_dia_serial_host_sse_float(const int num_rows,   #rdi
#                            const int num_cols,   #rsi
#                            const int num_diags,  #rdx
#                            const int stride,     #rcx
#                            const int       * offsets, #r8 
#                            const float * data,   #r9
#                            const float * x,      #stack
#                                  float * y)      #stack
__spmv_dia_serial_host_sse_float:
.LFB2:
	pushq	%r15
.LCFI0:
	movl	%edi, %r15d  #num_rows
	xorl	%edi, %edi   #
	pushq	%r14
.LCFI1:
	movl	%esi, %r14d  #num_cols
	pushq	%r13
.LCFI2:
	movq	%r8, %r13    #offset
	xorl	%r8d, %r8d   #
	pushq	%r12
.LCFI3:
	movl	%ecx, %r12d  #stride
	pushq	%rbp
.LCFI4:
	movq	%r9, %rbp   #data
	xorl	%r9d, %r9d
	pushq	%rbx
.LCFI5:
	movl	%edx, %ebx   #num_diags,
	.p2align 4,,7
.L17:
	cmpl	%ebx, %edi   ##
	jge	.L10
#.L19:
#	addl	$1, %r8d
#	cmpl	$99999, %r8d
#	jg	.L10
#	xorl	%edi, %edi     #outermost
#	cmpl	%ebx, %edi
#	jge	.L19
.L3:
	movslq	%edi,%rax               #index
	movl	%r15d, %r10d              #num_rows
	movl	(%r13,%rax,4), %edx       # offset
	movl	%r14d, %eax               #numcols
	movl	%edx, %ecx                #
	negl	%ecx                      #-offset[]
	testl	%ecx, %ecx
	cmovs	%r9d, %ecx                #max_int() i_start 
	testl	%edx, %edx
	cmovs	%r9d, %edx                #j_start
	subl	%ecx, %r10d
	movslq	%ecx,%rcx
	subl	%edx, %eax
	movslq	%edx,%rdx
	cmpl	%eax, %r10d
	cmovg	%eax, %r10d               #N
	movl	%edi, %eax
	imull	%r12d, %eax   
	cltq
	addq	%rcx, %rax
  subl  $3, %r10d
	testl	%r10d, %r10d
	leaq	(%rbp,%rax,4), %rsi       #data
	movq	56(%rsp), %rax            #
	leaq	(%rax,%rdx,4), %r11       #x
	movq	64(%rsp), %rax
	leaq	(%rax,%rcx,4), %rcx       #y
	xorl	%edx, %edx
	xorl	%eax, %eax
	jle	.L7
	.p2align 4,,7
################ inner loop #####################
.L6:
#	movss	(%rsi,%rax,4), %xmm0            ## data
#	addl	$1, %edx                        ##
#	mulss	(%r11,%rax,4), %xmm0            ##x
#	addss	(%rcx,%rax,4), %xmm0            ##y
#	movss	%xmm0, (%rcx,%rax,4)            ##y
#	addq	$1, %rax

  movups  (%rsi,%rax,4), %xmm0          ##data[1-4]
  mulps (%r11,%rax,4), %xmm0
  addps (%rcx,%rax,4),  %xmm0
  movups  %xmm0, (%rcx,%rax,4)

  addl  $4,%edx
  addq  $4, %rax

	cmpl	%r10d, %edx
	jne	.L6
################ end loop #####################
.L7:
  addl  $3, %r10d
  cmpl  %r10d, %edx
  jge .L4
.L8:
	movss	(%rsi,%rax,4), %xmm0
	addl	$1, %edx                        ##
	mulss	(%r11,%rax,4), %xmm0
	addss	(%rcx,%rax,4), %xmm0
	movss	%xmm0, (%rcx,%rax,4)
	addq	$1, %rax
	cmpl	%r10d, %edx
	jl	.L8
  
.L4:
	addl	$1, %edi
	jmp	.L17
.L10:
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
.LFE2:
	.size	__spmv_dia_serial_host_sse_float, .-__spmv_dia_serial_host_sse_float
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
	.byte	0x8f
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x8e
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI2-.LCFI1
	.byte	0xe
	.uleb128 0x20
	.byte	0x8d
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI3-.LCFI2
	.byte	0xe
	.uleb128 0x28
	.byte	0x8c
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI4-.LCFI3
	.byte	0xe
	.uleb128 0x30
	.byte	0x86
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xe
	.uleb128 0x38
	.byte	0x83
	.uleb128 0x7
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-44)"
	.section	.note.GNU-stack,"",@progbits
