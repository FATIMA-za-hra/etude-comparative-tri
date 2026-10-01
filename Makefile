CC = gcc
CFLAGS = -O2 -Wall -Wextra

tri: main.c tris.c mesure.c tris.h mesure.h
	$(CC) $(CFLAGS) -o tri main.c tris.c mesure.c

test: test_tris.c tris.c tris.h
	$(CC) $(CFLAGS) -o test_tris test_tris.c tris.c
	./test_tris

clean:
	rm -f tri test_tris

.PHONY: clean test