CC = gcc

SRCS = $(wildcard src/*.c src/**/*.c)
OBJS = $(patsubst src%, build%, $(SRCS:.c=.o))
DEPS = $(patsubst src%, build%, $(SRCS:.c=.d))

CPPFLAGS := -MMD -MP
CPPFLAGS += -Isrc
CPPFLAGS += -Ilib/glad/include
CPPFLAGS += -Ilib/stb
CPPFLAGS += -Ilib/SDL2/include
CPPFLAGS += -Ilib/cglm/include
CPPFLAGS += -I/usr/include/freetype2
CPPFLAGS += -I/usr/include/libpng16
CPPFLAGS += -D_DEBUG
CPPFLAGS += -D_EDITOR

CFLAGS := -std=c11
CFLAGS += -g
CFLAGS += -O2
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wno-unused-parameter
CFLAGS += -Wno-missing-braces
CFLAGS += $(CPPFLAGS)

LDLIBS := -lm
LDLIBS += -lSDL2
LDLIBS += -lglad
LDLIBS += -lcglm
LDLIBS += -lfreetype

LDFLAGS := -Llib/glad
LDFLAGS += -Llib/SDL2/build/build/.libs
LDFLAGS += -Llib/cglm/build
LDFLAGS += -Wl,--start-group $(LDLIBS) -Wl,--end-group

TARGET = bin/game

.PHONY: all clean

all: $(TARGET)

run: all
	@$(TARGET)

glad: 
	@cd lib/glad && $(CC) -o libglad.a -Iinclude -c src/glad.c

sdl:
	@cd lib/SDL2 && mkdir -p build && cd build && ../configure && make -j 6 

cglm:
	@cd lib/cglm && mkdir -p build && cd build && cmake .. && make -j 6 

libs: glad sdl cglm 

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)
