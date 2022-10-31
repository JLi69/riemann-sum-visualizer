CC=gcc
C_FLAG=-O2
LD_FLAG=-lGL -lglfw3 -lm -static-libgcc
BIN_NAME=integral

output:
	${CC} main.c glad.c ${C_FLAG} ${LD_FLAG} -o ${BIN_NAME}

run:
	./${BIN_NAME}

clean:
	rm -f ${BIN_NAME}
