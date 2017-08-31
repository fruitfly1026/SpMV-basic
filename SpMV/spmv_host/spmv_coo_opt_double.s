	.file	"spmv_coo.c"
	.text
	.p2align 4,,15
.globl __spmv_coo_serial_host_sse_double
	.type	__spmv_coo_serial_host_sse_double, @function
#void __spmv_coo_serial_host_sse_double(const int num_nonzeros,        ##rdi
#                            const int * rows,              ##rsi
#                            const int * cols,              ##rdx
#                            const double * data,           ##rcx     
#                            const double * x,              ##r8
#                                  double * y)              ##r9  
__spmv_coo_serial_host_sse_double:
.LFB2:
	pushq	%rbp
.LCFI0:
	xorl	%r10d, %r10d
	movq	%rsi, %rbp            ##
#	xorl	%r11d, %r11d          ##j spmv times
	movq	%rcx, %rsi
	pushq	%rbx
.LCFI1:
	movq	%rdx, %rbx
	.p2align 4,,7
.L13:
	xorl	%r10d, %r10d          ##i    
  subl  $3, %edi
	cmpl	%edi, %r10d           ##
	jge	.L8
  #######################loop begin ####################
.L3:
	movslq	%r10d,%rax           ## 
#	addl	$1, %r10d
#	movslq	(%rbx,%rax,4),%rcx   ##col[]
#	movsd	(%rsi,%rax,8), %xmm0   ##data[]
#	movslq	(%rbp,%rax,4),%rdx   ##row[]
#	mulsd	(%r8,%rcx,8), %xmm0    #x[]
#	leaq	(%r9,%rdx,8), %rdx     ##y addr
#	addsd	(%rdx), %xmm0          ##y[]
#	movsd	%xmm0, (%rdx)
####sse
  movdqu  (%rbx, %rax,4),%xmm8  #col[]
  movdqu  (%rbp,%rax,4), %xmm9  #row[]
  movupd  (%rsi,%rax,8), %xmm0  #data[] 1,2
  movupd  16(%rsi,%rax,8), %xmm15  #data[] 3,4
  movd  %xmm8, %ecx
  movsd   (%r8,%rcx,8), %xmm1   #x[1]
  pextrd  $1, %xmm8,%ecx
  movhpd   (%r8,%rcx,8), %xmm1   #x[2]
  pextrd  $2, %xmm8,%ecx
  movsd   (%r8,%rcx,8), %xmm3   #x[3]
  pextrd  $3, %xmm8,%ecx
  movhpd   (%r8,%rcx,8), %xmm3   #x[4]
  
#  movhpd  %xmm2,  %xmm1
#  movhpd  %xmm4,  %xmm3
  mulpd   %xmm1,  %xmm0
  mulpd   %xmm3,  %xmm15
  
  movd    %xmm9,  %edx
  movsd   %xmm0,  %xmm10
  addsd   (%r9,%rdx,8), %xmm10
  movsd   %xmm10, (%r9,%rdx,8)

  pextrd  $1,%xmm9,%edx
  movhlps %xmm0,  %xmm10  
  addsd   (%r9,%rdx,8), %xmm10
  movsd   %xmm10, (%r9,%rdx,8)

  pextrd  $2,%xmm9,%edx
  movsd   %xmm15,  %xmm10  
  addsd   (%r9,%rdx,8), %xmm10
  movsd   %xmm10, (%r9,%rdx,8)

  pextrd  $3,%xmm9,%edx
  movhlps %xmm15,  %xmm10  
  addsd   (%r9,%rdx,8), %xmm10
  movsd   %xmm10, (%r9,%rdx,8)

  addl  $4, %r10d
	cmpl	%edi, %r10d
	jl	.L3
  ####################loop end ###########################
.L8:
  addl  $3, %edi
  cmpl  %edi, %r10d
  jge .L15
.L9:
	movslq	%r10d,%rax           ## 
	addl	$1, %r10d
	movslq	(%rbx,%rax,4),%rcx   ##col[]
	movsd	(%rsi,%rax,8), %xmm0   ##data[]
	movslq	(%rbp,%rax,4),%rdx   ##row[]
	mulsd	(%r8,%rcx,8), %xmm0    #x[]
	leaq	(%r9,%rdx,8), %rdx     ##y addr
	addsd	(%rdx), %xmm0          ##y[]
	movsd	%xmm0, (%rdx)
	cmpl	%edi, %r10d
	jl	.L9

.L15:
#	addl	$1, %r11d
#	cmpl	$999, %r11d
#  jle .L13
.L7:
	popq	%rbx
	popq	%rbp
	ret
.LFE2:
	.size	__spmv_coo_serial_host_sse_double, .-__spmv_coo_serial_host_sse_double
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
	.ident	"GCC: (GNU) 4.1.2 20080704 (Red Hat 4.1.2-44)"
	.section	.note.GNU-stack,"",@progbits
