CC = clang 
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g
HEADERS = $(wildcard *.h)
MODULES = universe life
OBJECTS = $(TARGET).o $(patsubst %, %.o, ${MODULES})
TARGET = life

.PHONY: all done

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lncurses 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c 

clean: tidy
	$(RM) $(TARGET)

tidy:
	$(RM) $(OBJECTS)

format:
	clang-format -i -style=file *.[c,h]
                                                 

