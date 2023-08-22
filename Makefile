CC=gcc
OPT=-O0
SRC=src
OBJ=obj
DEP=dep

CFLAGS=-Wall -Wextra -g $(OPT)
LFLAGS=-lX11 -lGL -lm -logg -lvorbis -lvorbisfile -lasound

CFILES:=$(shell find $(SRC) -name '*.c')
HFILES:=$(shell find $(SRC) -name '*.h')
OFILES:=$(patsubst $(SRC)%,$(OBJ)%, $(patsubst %.c,%.o,$(CFILES)))

BINARY=bin

all: $(BINARY)

$(BINARY): $(OFILES)
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.c $(HFILES)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ)/* $(BINARY)

.PHONY: all clean
