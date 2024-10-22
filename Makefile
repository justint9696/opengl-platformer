CC = gcc

SRCS = $(wildcard src/*.c src/**/*.c)
OBJS = $(patsubst src/%, build/%, $(SRCS:.c=.o))
DEPS = $(patsubst src/%, build/%, $(SRCS:.c=.d))

INC_SRC = $(wildcard src/*.h src/**/*.h)
INC_DST = $(addprefix test/, $(patsubst src/%, include/%, $(INC_SRC)))

CFLAGS = -std=c11 \
		 -g \
		 -O2 \
		 -Wall \
		 -Wextra \
		 -Wundef \
		 -Wno-unused-parameter \
		 -Wno-missing-braces \
		 -Wno-unused-result \
		 -Isrc \
		 -Ilib/glad/include \
		 -Ilib/stb \
		 -I/usr/include/freetype2 \
		 -I/usr/include/libpng16 \
		 -D_DEBUG \
		 -D_EDITOR

LDFLAGS = -Llib/glad \
		  -lcglm \
		  -lglad \
		  -lSDL2 \
		  -lfreetype \
		  -lm

OUT = bin/game

.PHONY: all clean

all: $(OUT)

run: all
	@$(OUT)

libs:
	@cd lib/SDL2 && mkdir -p build && cd build && ../configure && make -j 6 && sudo make install
	@cd lib/cglm && mkdir -p build && cd build && cmake .. && make -j 6 && sudo make install
	@cd lib/glad && $(CC) -o libglad.a -Iinclude -c src/glad.c

$(OUT): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -c $< -MMD -MP $(CFLAGS)

clean:
	rm -f $(OUT) $(DEPS) $(OBJS)
	@if test -f test/Makefile; then (cd test; $(MAKE) $@); fi

.PHONY: $(INC_DST)
$(INC_DST):
	@mkdir -p $(dir $@)
	@cp -f $(patsubst test/include/%, src/%, $@) $@

tests: all $(INC_DST)
	@cp -r build test
	@if test -f test/Makefile; then (cd test; $(MAKE) run); fi

-include $(DEPS)
