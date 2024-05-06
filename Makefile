CC = gcc

BIN = bin/
OBJ = objs/
SRC = src/

SRCS = $(shell find $(SRC) -name "*.c")
OBJS = $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

CFLAGS := -g -O2 -Wall -Wextra
CFLAGS += -Wno-unused-parameter
CFLAGS += -Isrc
CFLAGS += -Ilib/glad/include
CFLAGS += -Ilib/stb

LDFLAGS := -lm
LDFLAGS += -Llib/glad
LDFLAGS += -lglad
LDFLAGS += $(shell sdl2-config --libs)

TARGET = $(BIN)game

.PHONY: all clean

all: $(TARGET)

run: all
	@$(TARGET)

libs: 
	@cd lib/glad && $(CC) -o libglad.a -Iinclude -c src/glad.c
	@cd lib/SDL2 && mkdir -p build && cd build && ../configure && make -j 6 && sudo make install
	@cd lib/cglm && mkdir -p build && cd build && cmake .. && make -j 6 && sudo make install

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
