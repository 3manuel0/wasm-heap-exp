# Compiler / tools
CC = clang
AR = llvm-ar

CFLAGS = -fno-builtin --target=wasm32 -ffast-math -O3 \
         --no-standard-libraries -I./include -DPLATFORM_WEB

LDFLAGS = --target=wasm32 -fno-builtin -ffast-math -O3 \
          --no-standard-libraries -I./include \
          -Wl,--export-all -Wl,--no-entry -Wl,--allow-undefined \
          -DPLATFORM_WEB


SRC = src/string.c src/wasm_memory.c src/arena.c
OBJ = $(SRC:.c=.o)

LIB = lib/lib3wasm.a
TARGET = build/main.wasm


all: $(TARGET)


src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	$(AR) rcs $(LIB) $(OBJ)

$(TARGET): main.c $(LIB)
	$(CC) $(LDFLAGS) main.c $(LIB) -o $(TARGET)


clean:
	rm -f src/*.o $(LIB) $(TARGET)
