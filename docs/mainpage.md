# cc65 Applesoft Floating Point Library

This project adds floating-point arithmetic support to software built with the cc65 toolchain for Apple II systems. Rather than inventing a separate numeric format, it wraps the built-in Applesoft ROM floating-point routines and uses the same 40-bit Apple II floating-point representation used by Applesoft.

The library is aimed at developers who want C programs to work naturally with floating-point values on Apple II hardware, while still compiling with cc65 and targeting the classic Apple II platform. It provides the types and functions needed to perform math operations, convert between C values and Apple II floating-point representation, and interact with string-based inputs and outputs.

In short, this project is a practical Apple II floating-point toolkit for cc65 developers: it brings Applesoft-compatible floating-point capability to C programs while staying faithful to the Apple II platform's native numeric model.

## Why it exists

The stock cc65 compiler suite does not include native support for C float or double types in the way a modern system does. This project fills that gap for Apple II development by exposing a practical floating-point library that speaks the language of the Apple II runtime environment with a very small memory footprint (approximately 600 bytes).

## Typical use

Applications can include the library, perform standard floating-point calculations from C, and store or print values using the Apple II floating-point format. The project also includes utilities to convert values to and from the binary representation expected by Applesoft, which makes it useful for both emulator and real-machine development.

## What the project provides

- Floating-point arithmetic: addition, subtraction, multiplication, and division
- Advanced math: sine, cosine, tangent, arctangent, logarithm, exponential, square root, and power
- Comparison and utility operations: sign, reciprocal, absolute value, and related helpers
- Conversion between C integers, strings, and Applesoft floating-point values
- Integration with Apple II software and firmware conventions through the Applesoft ROM routines
- Small memory footprint

## Project structure

The library source code, a single assembly language file and accompanying header file, are located in the `src` directory. The build script, `build.py`, automates the compilation and provides utility functions for converting between IEEE 754 and Applesoft float formats, etc.  

Documentation is doxygen based and is located in the `docs` directory.  The documentation build creates the directory `html`.  Open `index.html` in a web browser to view the documentation.

The build process creates a `build` directory.  It is all that is needed to use the library in another project.  Additionally, a disk image file (`.po`) is generated that can be useful for debugging, etc.

## Implementation notes

The library is a cc65 thin compatible wrapper written as a C header file and a 6502 assembly language file.  The library does use the Applesoft floating point routines in ROM.  As such, it uses a few resources that could conflict with a higher level application.  These are noted below.

### Zero page usage

The bindings utilize one zero page location ($FA) for state tracking along with
the standard zero page locations used by Applesoft.  The latter locations include
the FAC, ARG and temp variables: $9D-$A2, $A5-$AA, $93-$9C, $8A-$8E, $B1-$CD, $60-$61 and $A0-$A1.

### ROM/Language card bank switching

Many Apple II cc65 applications run with the language card bank enabled for additional 
memory.  This library wraps functions that make ROM calls with code that 
switches to ROM reading before making the call and restores the incoming bank selection
afterward.

Additionally, the library 'caches' any input arguments from C as they could be located
in the language card.  This is done in the traditional Apple input buffer: $0200.  Thus, 
several routines will destroy memory between $200 and $2FF.

## Reference documentation

Some reference documentation used to develop this library is available in the `docs` directory.
These files document the Applesoft entry points from Apple.

- Numeric layout details <a href="TIL00074.pdf" target="_blank">Floating Point Specification</a>.
- Basic numeric function details <a href="TIL00075.pdf" target="_blank">Core Math Functions</a>.
- I/O and other transform functions <a href="TIL00076.pdf" target="_blank">Utility Functions</a>.

## License and copyright

**Library version:** \asfp_version  
**Copyright:** © \asfp_year Randall Frank

This project is licensed under the MIT OpenSource license. See the file <a href="LICENSE">LICENSE</a> for details.
