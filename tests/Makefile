CC = gcc

BIN = bin
SRC = src

SRCS = $(shell find $(SRC) -name "*.c")
OBJS = $(patsubst $(SRC)%, $(BIN)%, $(SRCS:.c=))
DEPS = $(patsubst $(SRC)%, $(BIN)%, $(SRCS:.c=.d))

CFLAGS := -std=c2x
CFLAGS += -g -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpedantic
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-missing-braces

LDLIBS := -lm

LDFLAGS := -Wl,--start-group $(LDLIBS) -Wl,--end-group

all: $(OBJS)

$(BIN)/%: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ -MMD -MP $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

-include $(DEPS)
