#Omar, this way is so clean
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra 
#wild card replaced by space seperated list of existing files
HEADERS = $(wildcard *.h)
MODULES = quick insert batcher heap stats
#patsubst replaces text
OBJECTS = $(TARGET).o $(patsubst %, %.o, ${MODULES})
TARGET = sorting

#executes when make request
.PHONY: all done chief

all: $(TARGET)
#$^ takes $(OBJECTS) and puts it there
#$@ is name of what's on left

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ 

#$< accesses the first thing in a dependency list, so %.c
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c

clean: tidy
	$(RM) $(TARGET)

tidy:
	$(RM) $(OBJECTS)

format:
	clang-format -i -style=file *.[c,h]
