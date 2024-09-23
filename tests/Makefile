CC = gcc

SRCS = $(wildcard *.c)
OBJS = $(patsubst %, bin/%, $(SRCS:.c=.o))
DEPS = $(patsubst %, bin/%, $(SRCS:.c=.d))

CFLAGS := -std=c11
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Wno-missing-braces

LDLIBS := -lm

LDFLAGS := -Wl,--start-group $(LDLIBS) -Wl,--end-group

.PHONY: all clean

all: $(OBJS)

run: $(OBJS)
	@$(foreach file, $^, ./$(file);)

bin/%.o: %.c
	$(CC) -o $@ $^ -MMD -MP $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(DEPS)

-include $(DEPS)
