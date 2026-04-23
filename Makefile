include ../Makefile.inc

GEN_EXE = ex3-1 ex3-2 ex4-1 ex4-2 ex5-1 ex5-2 ex5-3 ex5-4 ex5-5 ex6-2 ex6-3 ex7-1 ex8-1 ex8-2 ex9-0 ex9-3 ex10-1 ex12-1 ex14-1 ex14-2

EXE = ${GEN_EXE} 

all : ${EXE}

allgen : ${GEN_EXE}


clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
