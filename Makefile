# Werkzeuge
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Optionen für Compiler und Linker
CFLAGS  = -mcpu=cortex-m4 -mthumb -O0 -g -Wall -Wextra
LDFLAGS = -mcpu=cortex-m4 -mthumb -nostdlib -T ld/linker.ld

# Dateien
SRC = src/main.c
OBJ = build/main.o
ELF = build/main.elf
BIN = build/main.bin

# "make" ohne Ziel baut das erste Ziel in der Datei
all: $(BIN)

# 1. Kompilieren: .c -> .o
$(OBJ): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# 2. Linken: .o -> .elf
$(ELF): $(OBJ) ld/linker.ld
	$(CC) $(LDFLAGS) $(OBJ) -o $@

# 3. Rohe Bytes ziehen: .elf -> .bin
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Flashen (baut vorher, falls nötig)
flash: $(BIN)
	st-flash --reset write $(BIN) 0x08000000

# Aufräumen
clean:
	rm -rf build

.PHONY: all flash clean
