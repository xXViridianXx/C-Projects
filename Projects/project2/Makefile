CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

# Makefile target
# <target name>: <target dependencies>
all: integrate

integrate:  functions.o mathlib.o integrate.o
	$(CC) $(CFLAGS) -o integrate functions.o mathlib.o integrate.o

integrate.o: integrate.c
	$(CC) $(CFLAGS) -c integrate.c

function.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c

mathlib.o: mathlib.c mathlib.h
	$(CC) $(CFLAGS) -c mathlib.c

clean:
	rm -f integrate *.o

format:
	clang-format -i -style=file *.[c,h]
