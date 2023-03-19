# Makefile for cmdline scriptures

script: scriptures.c
	gcc -O0 -g -Wall ${CFLAGS} scriptures.c -o lds

clean:
	rm -f lds

