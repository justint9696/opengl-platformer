CC = gcc

BIN = bin/
OBJ = obj/
SRC = src/

SRCS = $(shell find $(SRC) -name "*.c")
OBJS = $(patsubst $(SRC)%, $(OBJ)%, $(SRCS:.c=.o))

CFLAGS := -g -O2 -Wall -Wextra
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-missing-braces
CFLAGS += -Wno-sign-compare
CFLAGS += -Ilib/glad/include
CFLAGS += -Ilib/stb
CFLAGS += -Isrc
CFLAGS += $(shell sdl2-config --cflags)

LDFLAGS := -lm
LDFLAGS += -Llib/glad
LDFLAGS += -lglad
LDFLAGS += $(shell sdl2-config --libs)

TARGET = $(BIN)game

.PHONY: all clean

all: $(TARGET)

run: all
	@$(TARGET)

glad: 
	@cd lib/glad && $(CC) -o libglad.a -Iinclude -c src/glad.c

sdl:
	@cd lib/SDL2 && mkdir -p build && cd build && ../configure && make -j 6 && sudo make install

cglm:
	@cd lib/cglm && mkdir -p build && cd build && cmake .. && make -j 6 && sudo make install

libs: glad sdl cglm 

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ)%.o: $(SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(BIN) $(OBJ)
