CC = gcc

SRCS = $(wildcard src/*.c src/**/*.c)
OBJS = $(patsubst src/%, bin/%, $(SRCS:.c=.o))
DEPS = $(patsubst src/%, bin/%, $(SRCS:.c=.d))

CFLAGS := -std=c11
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-missing-braces

LDLIBS := -lm

LDFLAGS := -Wl,--start-group $(LDLIBS) -Wl,--end-group

.PHONY: all clean

all: $(OBJS)

bin/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ -MMD -MP $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(DEPS)

-include $(DEPS)
