CC=icc
#FLAG=-g -Wall
#FLAG=-O3
#LDFLAG=-O3
FLAG=-O3 -msse4.2 -fp-model source 
LDFLAG=-O3 -msse4.2 -fp-model source

KERNELS=spmv_host/spmv_dia_opt_single.o spmv_host/spmv_dia_opt_double.o \
				spmv_host/spmv_ell_opt_single.o spmv_host/spmv_ell_opt_double.o \
				spmv_host/spmv_csr_opt_single.o spmv_host/spmv_csr_opt_double.o \
				spmv_host/spmv_coo_opt_single.o spmv_host/spmv_coo_opt_double.o 
OBJS=driver.o mmio.o ${KERNELS} 

.cpp.o:
	${CC} -o $@ -c ${FLAG} $<
.s.o:
	as -o $@ $< 

spmv-icc: ${OBJS}
	${CC}  ${LDFLAG} -o $@ $^

.PHONY:clean
clean: 
	find ./ -name "*.o" -delete
	rm spmv-icc 

