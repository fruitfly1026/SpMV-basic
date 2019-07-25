CC=g++
#FLAG=-g -Wall
FLAG=-O3 -I./include/ -Wno-unused-result -Wno-write-strings
LDFLAG=-O3

OBJS=driver.o src/mmio.o 

.cpp.o:
	${CC} -o $@ -c ${FLAG} $<

spmv: ${OBJS}
	${CC}  ${LDFLAG} -o $@ $^

.PHONY:clean
clean: 
	find ./ -name "*.o" -delete
	rm spmv

