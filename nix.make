CC     := gcc
AR     := ar
LIB    := libvterm-ctrl.a
LIBS   := -lvterm-ctrl
CFLAGS := -g -Wall
ANSI   := -ansi -pedantic
SRC    := nix_test.c
EXE    := nix_test

all: library example
library:
	$(CC) $(CFLAGS) $(ANSI) -c vterm.c
	$(AR) rcs $(LIB) vterm.o
example:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) -L./ $(LIBS)
clean:
	-rm nix_test
	-rm -rf *.o
	-rm -rf *.elf
	-rm -rf *.hex
	-rm -rf *.a
	-rm -rf *.dSYM
