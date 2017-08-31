	.file	"spmv_coo.c"
	.text
	.p2align 4,,15
.globl __spmv_coo_serial_host_sse_float
	.type	__spmv_coo_serial_host_sse_float, @function
#void __spmv_coo_serial_host_sse_float(const int num_nonzeros,
#                            const int * rows, 
#                            const int * cols, 
#                            const float * data, 
#                            const float * x,  
#                                  float * y)
__spmv_coo_serial_host_sse_float:
.LFB2:
	pushq	%rbp
.LCFI0:
	xorl	%r10d, %r10d
	movq	%rsi, %rbp
	#xorl	%r11d, %r11d
	movq	%rcx, %rsi
	pushq	%rbx
.LCFI1:
	movq	%rdx, %rbx
	.p2align 4,,7
########## loop begin ############################################
.L13:
	xorl	%r10d, %r10d
  subl $3, %edi
	cmpl	%edi, %r10d
	jge	.L9
.L3:
#	movslq	%r10d,%rax        
#	addl	$1, %r10d                    #intermost loop
#	movslq	(%rbx,%rax,4),%rcx         #col addr
#	movss	(%rsi,%rax,4), %xmm0         #data 
#	movslq	(%rbp,%rax,4),%rdx         #row addr
#	mulss	(%r8,%rcx,4), %xmm0          #x
#	leaq	(%r9,%rdx,4), %rdx           #y addr
#	addss	(%rdx), %xmm0                
#	movss	%xmm0, (%rdx)                ##

####### sse ####################################
	movslq	%r10d,%rax        
  movdqu  (%rbx,%rax,4), %xmm8   #col[]
  movdqu  (%rbp,%rax,4), %xmm9   #row[]
  movups  (%rsi,%rax,4), %xmm0   ##value[]
  movd    %xmm8, %ecx
	movss	(%r8,%rcx,4), %xmm1
  pextrd  $1, %xmm8,%ecx
	movss	(%r8,%rcx,4), %xmm2
  pextrd  $2, %xmm8,%ecx
	movss	(%r8,%rcx,4), %xmm3
  pextrd  $3, %xmm8,%ecx
	movss	(%r8,%rcx,4), %xmm4

  insertps  $16, %xmm2, %xmm1        #x
  insertps  $32, %xmm3, %xmm1
  insertps  $48, %xmm4, %xmm1
  mulps  %xmm1,%xmm0                 #mul    
  ################right #######################
  movd  %xmm9, %edx
  #extractps  $0, %xmm0, %xmm10
  movss       %xmm0,  %xmm10
	addss      (%r9,%rdx,4),%xmm10
  movss       %xmm10, (%r9,%rdx,4)

  pextrd  $1, %xmm9,%edx
  movaps   %xmm0, %xmm10
  shufps  $1,%xmm0,%xmm10
  #extractps  $1, %xmm0,%xmm10
	addss      (%r9,%rdx,4),%xmm10
  movss       %xmm10, (%r9,%rdx,4)

  pextrd  $2, %xmm9,%edx
  #extractps  $2, %xmm0,%xmm10
  movaps   %xmm0, %xmm10
  shufps  $2,%xmm0,%xmm10
	addss      (%r9,%rdx,4),%xmm10
  movss       %xmm10, (%r9,%rdx,4)

  pextrd  $3, %xmm9,%edx
  #extractps  $3, %xmm0,%xmm10
  movaps   %xmm0, %xmm10
  shufps  $3, %xmm0,%xmm10
	addss      (%r9,%rdx,4),%xmm10
  movss       %xmm10, (%r9,%rdx,4)

  addl   $4, %r10d
################################################
  cmpl %edi,%r10d
  jl  .L3
######## loop end ###########################################
.L9:
  addl  $3, %edi
  cmpl  %edi, %r10d
  jge .L7
.L11:
	movslq	%r10d,%rax        
	addl	$1, %r10d                    #intermost loop
	movslq	(%rbx,%rax,4),%rcx         #col addr
	movss	(%rsi,%rax,4), %xmm0         #data
	movslq	(%rbp,%rax,4),%rdx         #row addr
	mulss	(%r8,%rcx,4), %xmm0          #
	leaq	(%r9,%rdx,4), %rdx           #y addr
	addss	(%rdx), %xmm0                
	movss	%xmm0, (%rdx)                ##
  cmpl %edi,%r10d
  jl  .L11

#.L10:
#	addl	$1, %r11d   #outermost index j
#	cmpl	$999, %r11d
#  jle  .L13

.L7:
	popq	%rbx
	popq	%rbp
	ret
.LFE2:
	.size	__spmv_coo_serial_host_sse_float, .-__spmv_coo_serial_host_sse_float
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
