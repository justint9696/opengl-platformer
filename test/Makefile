CC = gcc

SRCS = $(wildcard *.c)
OBJS = $(patsubst %, bin/%, $(SRCS:.c=.o))
DEPS = $(patsubst %, bin/%, $(SRCS:.c=.d))

CFLAGS = -std=c99 \
		 -g \
		 -Wall \
		 -Wextra \
		 -Wno-missing-braces \
		 -Iinclude

LDFLAGS = -lm

.PHONY: all clean

dirs:
	@mkdir -p bin build

all: dirs $(OBJS)

bin/%.o: %.c
	$(CC) -o $@ $< -MMD $(CFLAGS) $(LDFLAGS)

bin/allocator.o: allocator.c
	$(CC) -o $@ $< -MMD $(CFLAGS) $(LDFLAGS) build/data/allocator.o

run: all
	@for test in $(OBJS); do (echo ./$$test; ./$$test); done

clean:
	rm -f $(DEPS) $(OBJS)

-include $(DEPS)
