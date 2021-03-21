CC = gcc
CFLAGS = -Wall -g
EXECUTABLE = so-cpp
build:
	${CC} src/main/main.c src/data_structures/hash_map.c -o ${EXECUTABLE}

run:build
	./${EXECUTABLE}

memory-check:build
	valgrind --leak-check=full ./${EXECUTABLE}

clean:
	rm -rf *.o so-cpp