	.file	"spmv_ell.c"
	.text
	.p2align 4,,15
.globl __spmv_ell_serial_host_sse_double
	.type	__spmv_ell_serial_host_sse_double, @function
#void __spmv_ell_serial_host_sse_double(const int num_rows,          #rdi
#                            const int num_cols,          #rsi
#                            const int num_cols_per_row,  #rdx
#                            const int stride,            #rcx
#                            const int * Aj,              #r8
#                            const double * Ax,           #r9
#                            const double * x,            #stack 24()
#                                  double * y)            #stack 32()
#unused: r13,r14,r15  !!!
#IndexType num_rows, ---%edi 
#IndexType num_cols, ---%edx->%ebp 
#IndexType num_cols_per_row,
#IndexType stride, ---%ecx 
#IndexType * Aj, 
#ValueType * Ax, 
#ValueType * x, --x
#ValueType * y  --y
__spmv_ell_serial_host_sse_double:
.LFB2:
	pushq	%rbp
.LCFI0:
	movl	%edx, %ebp  
	testl	%ebp, %ebp
	pushq	%rbx
.LCFI1:
	movq	24(%rsp), %rdx   # x
	movq	32(%rsp), %r10   # y
    pushq   %r12
	jle	.L6
	movslq	%ecx,%rax        # stride
	xorl	%r11d, %r11d     #n
	leaq	0(,%rax,8), %rbx #Ax_n = 8*stride , can also use a shift operation instead!!
	leaq	0(,%rax,4), %rcx #Aj_n =4*stride, can also use a shift operation!!
	.p2align 4,,10
	.p2align 3
.L5:
#	jle	.L3
    subl    $1, %edi
  	testl	%edi, %edi       #num_rows
    jle  .L3
	xorl	%esi, %esi       #i
	.p2align 4,,10
	.p2align 3
.L4:
#	movslq	(%r8,%rsi,4),%rax     # Aj_n[i]
    movq    (%r8,%rsi,4),%mm0     #
#	movsd	(%r9,%rsi,8), %xmm0   # Ax_n[i]
	movupd	(%r9,%rsi,8), %xmm0   # Ax_n[i]
   #shift operation
    movd     %mm0, %r12d
	movsd  (%rdx,%r12,8), %xmm1  # x[Aj_n[i]]*Ax_n[i]
    psrlq   $32, %mm0
    movd     %mm0, %r12d
	movhpd  (%rdx,%r12,8), %xmm1  # x[Aj_n[i]]*Ax_n[i]
    mulpd   %xmm1, %xmm0 

#	mulsd	(%rdx,%rax,8), %xmm0  # x[Aj_n[i]]*Ax_n[i]
#	addsd	(%r10,%rsi,8), %xmm0  # y[i]+=...
	addpd	(%r10,%rsi,8), %xmm0  # y[i]+=...
#	movsd	%xmm0, (%r10,%rsi,8)  #
	movupd	%xmm0, (%r10,%rsi,8)  #
#	addq	$1, %rsi              # i+1
	addq	$2, %rsi              # i+1
	cmpl	%esi, %edi            #edi num_rows
	jg	.L4
.L3:
    addl    $1, %edi
    cmpl    %esi, %edi
    jle     .L7
#addede flag
.L8:
	movslq	(%r8,%rsi,4),%rax     # Aj_n[i]
	movsd	(%r9,%rsi,8), %xmm0   # Ax_n[i]
	mulsd	(%rdx,%rax,8), %xmm0  # x[Aj_n[i]]*Ax_n[i]
	addsd	(%r10,%rsi,8), %xmm0  # y[i]+=...
	movsd	%xmm0, (%r10,%rsi,8)  #
    addq    $1, %rsi
    cmpl    %esi, %edi
    jg      .L8
#addede flag
.L7:
	addl	$1, %r11d
	addq	%rbx, %r9            # Ax_n   
	addq	%rcx, %r8            # Aj_n
	cmpl	%r11d, %ebp          #if n<num_cols_per_row
	jg	.L5
.L6:
  popq    %r12
	popq	%rbx
	popq	%rbp
	ret
.LFE2:
	.size	__spmv_ell_serial_host_sse_double, .-__spmv_ell_serial_host_sse_double
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
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xe
	.uleb128 0x18
	.byte	0x83
	.uleb128 0x3
	.align 8
.LEFDE1:
	.ident	"GCC: (GNU) 4.3.4"
	.section	.note.GNU-stack,"",@progbits
