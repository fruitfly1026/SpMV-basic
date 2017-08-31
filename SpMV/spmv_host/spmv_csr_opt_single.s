	.file	"spmv_csr.c"
	.text
	.p2align 4,,15
.globl __spmv_csr_serial_host_sse_float
	.type	__spmv_csr_serial_host_sse_float, @function
#regester used :
#register unused:
#void __spmv_csr_serial_host_sse_float(const int num_rows,        //rdi
#                            const int * Ap, //row_start //rsi
#                            const int * Aj, //col_idx   //rdx 
#                            const float * Ax, //value   //rcx
#                            const float * x,    //r8
#                                  float * y)    //r9
#parameters:
__spmv_csr_serial_host_sse_float:
.LFB2:
	pushq	%r14
.LCFI0:
	movq	%rdx, %r14
	pushq	%r13
.LCFI1:
	movq	%rcx, %r13
	pushq	%r12
.LCFI2:
	movq	%r9, %r12
#	xorl	%r9d, %r9d
	pushq	%rbp
.LCFI3:
	movl	%edi, %ebp #num_rows
	pushq	%rbx
.LCFI4:
	movq	%rsi, %rbx
	xorl	%esi, %esi #i
	.p2align 4,,7
.L17:
	cmpl	%ebp, %esi
	jge	.L10
#.L19:
#	addl	$1, %r9d   #
#	cmpl	$999, %r9d
#	jg	.L10
#	xorl	%esi, %esi
#	cmpl	%ebp, %esi
#	jge	.L19
.L3:
	movslq	%esi,%rax  #i 
	leaq	0(,%rax,4), %rdx
	movl	(%rbx,%rax,4), %r10d #row_start[i] 
	movl	4(%rbx,%rdx), %ecx   #row_start[i+1]
	leaq	(%rdx,%r12), %rdi    #y[i] addr
  xorps %xmm8,%xmm8
  xorps %xmm10,%xmm10

	movss	(%rdi), %xmm1        #sum=y[i]
  subl  $3, %ecx             #unroll four
	movslq	%r10d,%rax         #row_start[i]
	salq	$2, %rax             #addr of row_start
	leaq	(%rax,%r14), %rdx    #col_idx  addr
	leaq	(%rax,%r13), %r11    #value addr
	cmpl	%ecx, %r10d          #
	jge	.L7
	.p2align 4,,7
########################main loop########################
.L6:
  movdqu  (%rdx),%xmm5
  movd  %xmm5,%eax
	movss	(%r8,%rax,4), %xmm0  #x[] 
  pextrd  $0b1,%xmm5,%eax
	movss	(%r8,%rax,4), %xmm2  #x[] 
  
  pextrd  $0b10,%xmm5,%eax
	movss	(%r8,%rax,4), %xmm3  #x[] 

  pextrd  $0b11,%xmm5,%eax
	movss	(%r8,%rax,4), %xmm4  #x[] 
#imm8[5-4]  
  insertps $16,%xmm2,%xmm0
  insertps $32, %xmm3,%xmm0
  insertps $48,%xmm4,%xmm0

  movups  (%r11), %xmm6   #value
  mulps %xmm6, %xmm0
  addps %xmm0, %xmm8 

  addl  $4, %r10d
  addq  $16, %rdx
  addq  $16,%r11
	cmpl	%r10d, %ecx          #
	jg	.L6
########################end loop########################
        movaps    %xmm8, %xmm10                                  #14.19
        movhlps   %xmm8, %xmm10                                 #14.19
        addps     %xmm10, %xmm8                                  #14.19
        movaps    %xmm8, %xmm9                                  #14.19
        shufps    $245, %xmm8, %xmm9             #3,3,1,1       #14.19
        addss     %xmm9, %xmm8                                  #14.19
        xorps     %xmm10, %xmm10
        movss     %xmm8, %xmm10
     #   movaps    %xmm8, %xmm10                                  #14.19
.L7:
  addl  $3, %ecx 
.L8:
	cmpl	%r10d, %ecx          #
  jle .L4
.L9:
	movslq	(%rdx),%rax  #col_idx[jj]
	addl	$1, %r10d      #
	addq	$4, %rdx
	movss	(%r8,%rax,4), %xmm0  #x[] 
	mulss	(%r11), %xmm0        #value
	addq	$4, %r11             # 
	cmpl	%r10d, %ecx          #
	addss	%xmm0, %xmm10         #sum
	jg	.L9
.L4:
	addl	$1, %esi
  addss %xmm1, %xmm10
	movss	%xmm10, (%rdi)        #y[i]
	jmp	.L17
.L10:
	popq	%rbx
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	ret
.LFE2:
	.size	__spmv_csr_serial_host_sse_float, .-__spmv_csr_serial_host_sse_float
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
