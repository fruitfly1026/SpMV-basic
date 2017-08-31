	.file	"spmv_csr.c"
	.text
	.p2align 4,,15
.globl __spmv_csr_serial_host_sse_double
	.type	__spmv_csr_serial_host_sse_double, @function
.LFB0:
__spmv_csr_serial_host_sse_double:
#void __spmv_csr_serial_host_sse_double(const int num_rows,       #di
#                            const int * Ap, //row_start #si
#                            const int * Aj, //col_idx  #dx
#                            const double * Ax, //value #cx
#                            const double * x,    #r8
#                                  double * y)    #r9
# -mavx -O2
#register unused: r15,bx
	.cfi_startproc
	#pushq	%r15
#	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	leal	-1(%rdi), %eax
#	xorl	%r14d, %r14d
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	leaq	4(,%rax,4), %r13    #rax*4+4
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	movq	%r9, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
.L2:
	testl	%edi, %edi
	jle	.L7
	xorl	%ebp, %ebp
	.p2align 4,,10
	.p2align 3
.L5:
	movl	(%rsi,%rbp), %eax   #row_start[i] row_start
	movl	4(%rsi,%rbp), %ebx  #row_start[i+1] row_end
#	decl	%ebx                #at least two element are required
	subl	$7, %ebx
#	xorpd	%xmm1, %xmm1
	movsd	(%r12,%rbp,2), %xmm1 # y[i] initialize as y[i] not zero
	movslq	%eax, %r9       #row_start
	leaq	(%rdx,%r9,4), %r11 #addr of col_idx[jj]
	leaq	(%rcx,%r9,8), %r10 #addr of value[jj]
	cmpl	%ebx, %eax      # 
	jge	.L8
	.p2align 4,,10
	.p2align 3
.L4:
	movdqu	(%r11),%xmm5     #col_idx[]-----four cols 1-4
	movdqu	16(%r11),%xmm13  #col_idx[]-----four cols  5-8
	movupd	(%r10), %xmm0   #value[] --------two values 1-2
	movupd	16(%r10), %xmm4   #value[] --------two values 3-4


	movd	%xmm5, %r9d
	movsd	(%r8,%r9,8), %xmm2     #x[col_idx[jj]]

	pextrd  $0b1,%xmm5,%r9d
	movhpd	(%r8,%r9,8), %xmm2     #x[col_idx[jj]]

   	pextrd  $0b10,%xmm5,%r9d
	movsd	(%r8,%r9,8), %xmm6     #x[col_idx[jj]]

   	pextrd  $0b11,%xmm5,%r9d
	movhpd	(%r8,%r9,8), %xmm6     #x[col_idx[jj]]

	movupd	32(%r10), %xmm8   #value[] --------two values  5-6
	movupd	48(%r10), %xmm12   #value[] --------two values 7-8

	addl	$8,%eax
	addq	$32,%r11
	addq	$64, %r10

	mulpd	%xmm2,  %xmm0
	mulpd	%xmm6,  %xmm4

#########seond #####################	
	movd	%xmm13, %r9d
	movsd	(%r8,%r9,8), %xmm10     #x[col_idx[jj]]

	pextrd  $0b1,%xmm13,%r9d
	movhpd	(%r8,%r9,8), %xmm10     #x[col_idx[jj]]

   	pextrd  $0b10,%xmm13,%r9d
	movsd	(%r8,%r9,8), %xmm14     #x[col_idx[jj]]

   	pextrd  $0b11,%xmm13,%r9d
	movhpd	(%r8,%r9,8), %xmm14     #x[col_idx[jj]]
#######################################

	mulpd	%xmm10,	%xmm8
	mulpd	%xmm14,	%xmm12

	addpd	%xmm0, %xmm1
	addpd	%xmm4, %xmm1
	addpd	%xmm8,	%xmm1
	addpd	%xmm12,%xmm1
	cmpl	%eax, %ebx             #
	jg	.L4
.L8:
	addl	$7, %ebx
.L10:
	cmpl    %ebx,%eax 
	jge 	.L3
	movslq	(%r11), %r9 #col_idx[jj]
	addl	$1, %eax   
	addq	$4, %r11
	movsd	(%r8,%r9,8), %xmm0     #x[col_idx[jj]]
	mulsd	(%r10), %xmm0          #value[jj]*...
	addq	$8, %r10
	addsd	%xmm0, %xmm1
	jmp .L10
.L3:
	movhlps	%xmm1,%xmm3
	addsd	%xmm3,%xmm1
	movsd	%xmm1, (%r12,%rbp,2)
	addq	$4, %rbp        # outer index
	cmpq	%r13, %rbp
	jne	.L5
.L7:
#	addl	$1, %r14d     #n 
#	cmpl	$100000, %r14d
#	jne	.L2
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
#	popq	%r14
	.cfi_def_cfa_offset 8
	#popq	%r15
	ret
	.cfi_endproc
.LFE0:
	.size	__spmv_csr_serial_host_sse_double, .-__spmv_csr_serial_host_sse_double
	.ident	"GCC: (GNU) 4.5.1 20100924 (Red Hat 4.5.1-4)"
	.section	.note.GNU-stack,"",@progbits
