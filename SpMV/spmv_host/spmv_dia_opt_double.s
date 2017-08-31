# -- Machine type EFI2
# mark_description "Intel(R) C Intel(R) 64 Compiler XE for applications running on Intel(R) 64, Version 12.0.2.137 Build 2011011";
# mark_description "2";
# mark_description "-xsse4.2 -S -o spmv_dia_icc_xsse_nounroll.s";
	.file "spmv_dia.c"
	.text
..TXTST0:
# -- Begin  __spmv_dia_serial_host_sse_double
#on sandybirdge,bw eff =60%
# mark_begin;
       .align    16,0x90
	.globl __spmv_dia_serial_host_sse_double
__spmv_dia_serial_host_sse_double:
# parameter 1: %edi
# parameter 2: %esi
# parameter 3: %edx
# parameter 4: %ecx
# parameter 5: %r8
# parameter 6: %r9
# parameter 7: 56 + %rsp
# parameter 8: 64 + %rsp
..B1.1:                         # Preds ..B1.0
..___tag_value___spmv_dia_serial_host_sse_double.1:                        #11.1
        pushq     %r12                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.3:                        #
        pushq     %r13                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.5:                        #
        pushq     %r14                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.7:                        #
        pushq     %r15                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.9:                        #
        pushq     %rbx                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.11:                       #
        pushq     %rbp                                          #11.1
..___tag_value___spmv_dia_serial_host_sse_double.13:                       #
#        xorl      %eax, %eax                                    #13.3
        movq      %r9, -40(%rsp)                                #13.3
        xorl      %r10d, %r10d                                  #13.3
                                # LOE r8 eax edx ecx esi edi r10d
..B1.2:                         # Preds ..B1.38 ..B1.43 ..B1.1
        movl      %r10d, %r14d                                  #14.5
        xorl      %r13d, %r13d                                  #
        testl     %edx, %edx                                    #14.20
        jle       ..B1.43       # Prob 10%                      #14.20
                                # LOE r8 r13 eax edx ecx esi edi r10d r14d
..B1.3:                         # Preds ..B1.2
#        movl      %eax, -32(%rsp)                               #
                                # LOE r8 r13 edx ecx esi edi r10d r14d
..B1.4:                         # Preds ..B1.37 ..B1.3
        movl      (%r8,%r13,4), %r12d                           #3.6
        movl      %r12d, %ebp                                   #17.29
        negl      %ebp                                          #17.29
        testl     %ebp, %ebp                                    #17.29
        cmovle    %r10d, %ebp                                   #17.29
        testl     %r12d, %r12d                                  #18.29
        movslq    %ebp, %rax                                    #17.29
        cmovle    %r10d, %r12d                                  #18.29
        negl      %ebp                                          #21.23
        movslq    %r12d, %rbx                                   #18.29
        negl      %r12d                                         #21.23
        addl      %edi, %ebp                                    #21.23
        addl      %esi, %r12d                                   #21.23
        cmpl      %r12d, %ebp                                   #21.23
        cmovl     %ebp, %r12d                                   #21.23
        movslq    %r12d, %r11                                   #21.23
        testq     %r11, %r11                                    #34.19
        jle       ..B1.37       # Prob 50%                      #34.19
                                # LOE rax rbx r8 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.5:                         # Preds ..B1.4
        movl      %ecx, %r9d                                    #23.38
        imull     %r14d, %r9d                                   #23.38
        movslq    %r9d, %r9                                     #23.29
        movq      -40(%rsp), %rbp                               #23.27
        lea       (%rbp,%r9,8), %r15                            #23.27
        movq      64(%rsp), %r9                                 #25.27
        lea       (%r15,%rax,8), %rbp                           #23.27
        movq      %rbp, -16(%rsp)                               #23.27
        lea       (%r9,%rax,8), %r9                             #25.27
        cmpq      %rbp, %r9                                     #35.22
        jbe       ..B1.7        # Prob 50%                      #35.22
                                # LOE rbx r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.6:                         # Preds ..B1.5
        movq      %r9, %rbp                                     #35.22
        lea       (,%r11,8), %rax                               #35.22
        subq      -16(%rsp), %rbp                               #35.22
        cmpq      %rbp, %rax                                    #35.22
        jle       ..B1.9        # Prob 50%                      #35.22
                                # LOE rax rbx r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.7:                         # Preds ..B1.5 ..B1.6
        cmpq      -16(%rsp), %r9                                #35.22
        jae       ..B1.33       # Prob 50%                      #35.22
                                # LOE rbx r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.8:                         # Preds ..B1.7
        movq      -16(%rsp), %rbp                               #35.22
        lea       (,%r11,8), %rax                               #35.22
        subq      %r9, %rbp                                     #35.22
        cmpq      %rax, %rbp                                    #35.22
        jl        ..B1.33       # Prob 50%                      #35.22
                                # LOE rax rbx r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.9:                         # Preds ..B1.8 ..B1.6
        movq      56(%rsp), %rbp                                #24.27
        lea       (%rbp,%rbx,8), %rbp                           #24.27
        cmpq      %rbp, %r9                                     #35.30
        jbe       ..B1.11       # Prob 50%                      #35.30
                                # LOE rax rbp r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.10:                        # Preds ..B1.9
        movq      %r9, %rbx                                     #35.30
        subq      %rbp, %rbx                                    #35.30
        cmpq      %rbx, %rax                                    #35.30
        jle       ..B1.13       # Prob 50%                      #35.30
                                # LOE rax rbp r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.11:                        # Preds ..B1.9 ..B1.10
        cmpq      %r9, %rbp                                     #35.30
        jbe       ..B1.34       # Prob 50%                      #35.30
                                # LOE rax rbp r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.12:                        # Preds ..B1.11
        movq      %rbp, %rbx                                    #35.30
        subq      %r9, %rbx                                     #35.30
        cmpq      %rax, %rbx                                    #35.30
        jl        ..B1.34       # Prob 50%                      #35.30
                                # LOE rbp r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.13:                        # Preds ..B1.10 ..B1.12
        movq      %r9, %rax                                     #34.9
        andq      $15, %rax                                     #34.9
        testl     %eax, %eax                                    #34.9
        je        ..B1.16       # Prob 50%                      #34.9
                                # LOE rbp r8 r9 r11 r13 eax edx ecx esi edi r10d r12d r14d
..B1.14:                        # Preds ..B1.13
        testb     $7, %al                                       #34.9
        jne       ..B1.41       # Prob 10%                      #34.9
                                # LOE rbp r8 r9 r11 r13 edx ecx esi edi r10d r12d r14d
..B1.15:                        # Preds ..B1.14
        movl      $1, %eax                                      #34.9
                                # LOE rbp r8 r9 r11 r13 eax edx ecx esi edi r10d r12d r14d
..B1.16:                        # Preds ..B1.15 ..B1.13
        movl      %eax, %ebx                                    #34.9
        lea       8(%rbx), %r15                                 #34.9
        cmpq      %r15, %r11                                    #34.9
        jl        ..B1.41       # Prob 10%                      #34.9
                                # LOE rbx rbp r8 r9 r11 r13 eax edx ecx esi edi r10d r12d r14d
..B1.17:                        # Preds ..B1.16
        movl      %r12d, %r15d                                  #34.9
        subl      %eax, %r15d                                   #34.9
        andl      $7, %r15d                                     #34.9
        subl      %r15d, %r12d                                  #34.9
        xorl      %r15d, %r15d                                  #34.9
        movslq    %r12d, %r12                                   #34.9
        testq     %rbx, %rbx                                    #34.9
        jbe       ..B1.21       # Prob 10%                      #34.9
                                # LOE rbx rbp r8 r9 r11 r12 r13 r15 eax edx ecx esi edi r10d r14d
..B1.18:                        # Preds ..B1.17
        movq      %r8, -24(%rsp)                                #
        movq      -16(%rsp), %r8                                #
                                # LOE rbx rbp r8 r9 r11 r12 r13 r15 eax edx ecx esi edi r10d r14d
..B1.19:                        # Preds ..B1.19 ..B1.18
        movsd     (%r8,%r15,8), %xmm0                           #23.27
        mulsd     (%rbp,%r15,8), %xmm0                          #35.30
        addsd     (%r9,%r15,8), %xmm0                           #35.13
        movsd     %xmm0, (%r9,%r15,8)                           #25.27
        incq      %r15                                          #34.9
        cmpq      %rbx, %r15                                    #34.9
        jb        ..B1.19       # Prob 82%                      #34.9
                                # LOE rbx rbp r8 r9 r11 r12 r13 r15 eax edx ecx esi edi r10d r14d
..B1.20:                        # Preds ..B1.19
        movq      -24(%rsp), %r8                                #
                                # LOE rbx rbp r8 r9 r11 r12 r13 eax edx ecx esi edi r10d r14d
..B1.21:                        # Preds ..B1.17 ..B1.20
        movl      %eax, %eax                                    #35.30
        lea       (%rbp,%rax,8), %r15                           #24.27
        testq     $15, %r15                                     #34.9
        je        ..B1.25       # Prob 60%                      #34.9
                                # LOE rbx rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.22:                        # Preds ..B1.21
        movq      -16(%rsp), %rax                               #
        .align    16,0x90
                                # LOE rax rbx rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.23:                        # Preds ..B1.23 ..B1.22
        movups    (%rax,%rbx,8), %xmm1                          #23.27
        movups    (%rbp,%rbx,8), %xmm0                          #24.27
        mulpd     %xmm0, %xmm1                                  #35.30
        addpd     (%r9,%rbx,8), %xmm1                           #35.13
        movaps    %xmm1, (%r9,%rbx,8)                           #25.27
        movups    16(%rax,%rbx,8), %xmm3                        #23.27
        movups    16(%rbp,%rbx,8), %xmm2                        #24.27
        mulpd     %xmm2, %xmm3                                  #35.30
        addpd     16(%r9,%rbx,8), %xmm3                         #35.13
        movaps    %xmm3, 16(%r9,%rbx,8)                         #25.27
        movups    32(%rax,%rbx,8), %xmm5                        #23.27
        movups    32(%rbp,%rbx,8), %xmm4                        #24.27
        mulpd     %xmm4, %xmm5                                  #35.30
        addpd     32(%r9,%rbx,8), %xmm5                         #35.13
        movaps    %xmm5, 32(%r9,%rbx,8)                         #25.27
        movups    48(%rax,%rbx,8), %xmm7                        #23.27
        movups    48(%rbp,%rbx,8), %xmm6                        #24.27
        mulpd     %xmm6, %xmm7                                  #35.30
        addpd     48(%r9,%rbx,8), %xmm7                         #35.13
        movaps    %xmm7, 48(%r9,%rbx,8)                         #25.27
        addq      $8, %rbx                                      #34.9
        cmpq      %r12, %rbx                                    #34.9
        jb        ..B1.23       # Prob 82%                      #34.9
        jmp       ..B1.28       # Prob 100%                     #34.9
                                # LOE rax rbx rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.25:                        # Preds ..B1.21
        movq      -16(%rsp), %rax                               #
        .align    16,0x90
                                # LOE rax rbx rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.26:                        # Preds ..B1.26 ..B1.25
        movups    (%rax,%rbx,8), %xmm0                          #23.27
        mulpd     (%rbp,%rbx,8), %xmm0                          #35.30
        addpd     (%r9,%rbx,8), %xmm0                           #35.13
        movaps    %xmm0, (%r9,%rbx,8)                           #25.27
        movups    16(%rax,%rbx,8), %xmm1                        #23.27
        mulpd     16(%rbp,%rbx,8), %xmm1                        #35.30
        addpd     16(%r9,%rbx,8), %xmm1                         #35.13
        movaps    %xmm1, 16(%r9,%rbx,8)                         #25.27
        movups    32(%rax,%rbx,8), %xmm2                        #23.27
        mulpd     32(%rbp,%rbx,8), %xmm2                        #35.30
        addpd     32(%r9,%rbx,8), %xmm2                         #35.13
        movaps    %xmm2, 32(%r9,%rbx,8)                         #25.27
        movups    48(%rax,%rbx,8), %xmm3                        #23.27
        mulpd     48(%rbp,%rbx,8), %xmm3                        #35.30
        addpd     48(%r9,%rbx,8), %xmm3                         #35.13
        movaps    %xmm3, 48(%r9,%rbx,8)                         #25.27
        addq      $8, %rbx                                      #34.9
        cmpq      %r12, %rbx                                    #34.9
        jb        ..B1.26       # Prob 82%                      #34.9
                                # LOE rax rbx rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.28:                        # Preds ..B1.26 ..B1.23 ..B1.41
        cmpq      %r11, %r12                                    #34.9
        jae       ..B1.37       # Prob 10%                      #34.9
                                # LOE rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.29:                        # Preds ..B1.28
        movq      -16(%rsp), %rax                               #
                                # LOE rax rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.30:                        # Preds ..B1.30 ..B1.29
        movsd     (%rax,%r12,8), %xmm0                          #23.27
        mulsd     (%rbp,%r12,8), %xmm0                          #35.30
        addsd     (%r9,%r12,8), %xmm0                           #35.13
        movsd     %xmm0, (%r9,%r12,8)                           #25.27
        incq      %r12                                          #34.9
        cmpq      %r11, %r12                                    #34.9
        jb        ..B1.30       # Prob 82%                      #34.9
        jmp       ..B1.37       # Prob 100%                     #34.9
                                # LOE rax rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.33:                        # Preds ..B1.8 ..B1.7
        movq      56(%rsp), %rax                                #24.27
        lea       (%rax,%rbx,8), %rbp                           #24.27
                                # LOE rbp r8 r9 r11 r13 edx ecx esi edi r10d r14d
..B1.34:                        # Preds ..B1.12 ..B1.11 ..B1.33
        movq      -16(%rsp), %rbx                               #34.9
        xorl      %eax, %eax                                    #34.9
                                # LOE rax rbx rbp r8 r9 r11 r13 edx ecx esi edi r10d r14d
..B1.35:                        # Preds ..B1.35 ..B1.34
        movsd     (%rbx,%rax,8), %xmm0                          #23.27
        mulsd     (%rbp,%rax,8), %xmm0                          #35.30
        addsd     (%r9,%rax,8), %xmm0                           #35.13
        movsd     %xmm0, (%r9,%rax,8)                           #25.27
        incq      %rax                                          #34.9
        cmpq      %r11, %rax                                    #34.9
        jb        ..B1.35       # Prob 82%                      #34.9
                                # LOE rax rbx rbp r8 r9 r11 r13 edx ecx esi edi r10d r14d
..B1.37:                        # Preds ..B1.35 ..B1.30 ..B1.4 ..B1.28
        incl      %r14d                                         #14.5
        incq      %r13                                          #14.5
        cmpl      %edx, %r14d                                   #14.5
        jb        ..B1.4        # Prob 82%                      #14.5
                                # LOE r8 r13 edx ecx esi edi r10d r14d
..B1.38:                        # Preds ..B1.37
        .byte     15                                            #
        .byte     31                                            #
        .byte     0                                             #
#        movl      -32(%rsp), %eax                               #
#        incl      %eax                                          #13.3
#        cmpl      $100000, %eax                                 #13.3
#        jb        ..B1.2        # Prob 99%                      #13.3
                                # LOE r8 eax edx ecx esi edi r10d
..B1.40:                        # Preds ..B1.43 ..B1.38         # Infreq
..___tag_value___spmv_dia_serial_host_sse_double.15:                       #37.1
        popq      %rbp                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.16:                       #
        popq      %rbx                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.18:                       #
        popq      %r15                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.20:                       #
        popq      %r14                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.22:                       #
        popq      %r13                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.24:                       #
        popq      %r12                                          #37.1
..___tag_value___spmv_dia_serial_host_sse_double.26:                       #
        ret                                                     #37.1
..___tag_value___spmv_dia_serial_host_sse_double.27:                       #
                                # LOE
..B1.41:                        # Preds ..B1.14 ..B1.16         # Infreq
        xorl      %r12d, %r12d                                  #34.9
        jmp       ..B1.28       # Prob 100%                     #34.9
                                # LOE rbp r8 r9 r11 r12 r13 edx ecx esi edi r10d r14d
..B1.43:                        # Preds ..B1.2                  # Infreq
#        incl      %eax                                          #13.3
#        cmpl      $100000, %eax                                 #13.3
#        jb        ..B1.2        # Prob 99%                      #13.3
        jmp       ..B1.40       # Prob 100%                     #13.3
        .align    16,0x90
..___tag_value___spmv_dia_serial_host_sse_double.34:                       #
                                # LOE r8 eax edx ecx esi edi r10d
# mark_end;
	.type	__spmv_dia_serial_host_sse_double,@function
	.size	__spmv_dia_serial_host_sse_double,.-__spmv_dia_serial_host_sse_double
	.data
# -- End  __spmv_dia_serial_host_sse_double
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
	.4byte 0x00000014
	.8byte 0x7801000100000000
	.8byte 0x0000019008070c10
	.4byte 0x00000000
	.4byte 0x000000ac
	.4byte 0x0000001c
	.8byte ..___tag_value___spmv_dia_serial_host_sse_double.1
	.8byte ..___tag_value___spmv_dia_serial_host_sse_double.34-..___tag_value___spmv_dia_serial_host_sse_double.1
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.3-..___tag_value___spmv_dia_serial_host_sse_double.1
	.4byte 0x070c028c
	.2byte 0x0410
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.5-..___tag_value___spmv_dia_serial_host_sse_double.3
	.4byte 0x070c038d
	.2byte 0x0418
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.7-..___tag_value___spmv_dia_serial_host_sse_double.5
	.4byte 0x070c048e
	.2byte 0x0420
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.9-..___tag_value___spmv_dia_serial_host_sse_double.7
	.4byte 0x070c058f
	.2byte 0x0428
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.11-..___tag_value___spmv_dia_serial_host_sse_double.9
	.4byte 0x070c0683
	.2byte 0x0430
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.13-..___tag_value___spmv_dia_serial_host_sse_double.11
	.4byte 0x070c0786
	.2byte 0x0438
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.15-..___tag_value___spmv_dia_serial_host_sse_double.13
	.4byte 0x04060609
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.16-..___tag_value___spmv_dia_serial_host_sse_double.15
	.4byte 0x0930070c
	.2byte 0x0303
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.18-..___tag_value___spmv_dia_serial_host_sse_double.16
	.4byte 0x0928070c
	.2byte 0x0f0f
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.20-..___tag_value___spmv_dia_serial_host_sse_double.18
	.4byte 0x0920070c
	.2byte 0x0e0e
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.22-..___tag_value___spmv_dia_serial_host_sse_double.20
	.4byte 0x0918070c
	.2byte 0x0d0d
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.24-..___tag_value___spmv_dia_serial_host_sse_double.22
	.4byte 0x0910070c
	.2byte 0x0c0c
	.byte 0x04
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.26-..___tag_value___spmv_dia_serial_host_sse_double.24
	.4byte 0x0408070c
	.4byte ..___tag_value___spmv_dia_serial_host_sse_double.27-..___tag_value___spmv_dia_serial_host_sse_double.26
	.8byte 0x8c0786068338070c
	.8byte 0x00058f048e038d02
# End
