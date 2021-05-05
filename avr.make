NAME   := firm
ELF    := $(NAME).elf
BIN    := $(NAME).hex
LIB    := libvterm-ctrl.a
CFLAGS := -g -Os -DLVTC_EMBEDDED
ANSI   := -ansi -pedantic
MMCU   := atmega328p
PROG   := buspirate
PORT   := /dev/tty.usbserial-AL03OOL4
MCU    := m328p
FOPT   := -V

all: library example
library:
	avr-gcc $(CFLAGS) $(ANSI) -mmcu=$(MMCU) -c vterm.c
	avr-ar rcs $(LIB) vterm.o
example:
	avr-gcc $(CFLAGS) -mmcu=$(MMCU) -c avr_test.c
	avr-gcc $(CFLAGS) -mmcu=$(MMCU) avr_test.o -o $(ELF) $(LIB)
	avr-objcopy -j .text -j .data -O ihex $(ELF) $(BIN)
flash:
	avrdude $(FOPT) -c $(PROG) -P $(PORT) -p $(MCU) -U flash:w:$(BIN)
clean:
	-rm -rf *.o
	-rm -rf *.elf
	-rm -rf *.hex
	-rm -rf *.a
	-rm -rf *.dSYM
