CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g
HEADERS = $(wildcard *.h)
MODULES = bf bv ht pq node parser speck text
OBJECTS = $(IDENTIFY).o $(patsubst %, %.o, ${MODULES})
IDENTIFY = identify


.PHONY: all done

all: IDENTIFY


IDENTIFY: $(IDENTIFY)


$(IDENTIFY): $(OBJECTS)
	$(CC) -o $@ $^ -lm


%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

clean: tidy
	$(RM) $(IDENTIFY)

tidy:
	$(RM) $(OBJECTS)

format:
	clang-format -i -style=file *.[c,h]
