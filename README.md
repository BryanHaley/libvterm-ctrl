# libvterm-ctrl
Libvterm-ctrl is a small ANSI C static library to control terminals compatible with ANSI/ISO/VT-100 control sequences.

Its main target platforms are microcontrollers where curses implementations are often not available and/or not desirable to integrate into a project, but output to a terminal with more than basic text is useful.

## Usage

libvterm-ctrl does not assume the terminal is binded to stdin/stdout, so the library must be initialized with the following function.

```void vt_init (FILE *input, FILE *output);```

Note that this function can be called again at any time if changing the input or output files is desired.

Afterwards, simply call any of the functions available in `vterm.h` to use them. Note that terminals are not guarenteed to support all available functions.

Any escape sequences not wrapped in functions by this library can be sent using the `vt_send` or `vt_nsend` functions.

## Compiling

Libvterm-ctrl can be compiled as a static library, or you may simply add `vterm.h` and `vterm.c` to your project. 

An example makefile and program are provided for an ATMEGA-328P and Desktop Mac/Linux (`avr_test.c, avr.make` and `nix_test.c, makefile` respectively). The avr test build environment assumes an AVR toolchain (namely `avr-gcc`, `avr-ar`, `avr-objcopy`, and optionally `avrdude` for flashing the program) is installed and available on the host machine. `run.sh` is provided as an example of flashing the program onto the microcontroller and using GNU Screen to receive input and output.

The example program shows binding the ATMEGA-328P UART port to stdin/stdout (in the case of the AVR example), passing stdin/stdout to libvterm-ctrl, and creating a simple command line interface that can be used to test some features of the library.

## License

Copyright 2018-2021 Bryan Haley under the MIT License. 

See LICENSE.MD for more information.
