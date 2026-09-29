# cc65 Applesoft Floating Point Library

This project adds floating-point arithmetic support to software built with the cc65 toolchain for Apple II systems. Rather than inventing a separate numeric format, it wraps the built-in Applesoft ROM floating-point routines and uses the same 40-bit Apple II floating-point representation used by Applesoft.

The library is aimed at developers who want C programs to work naturally with floating-point values on Apple II hardware, while still compiling with cc65 and targeting the classic Apple II platform. It provides the types and functions needed to perform math operations, convert between C values and Apple II floating-point representation, and interact with string-based inputs and outputs.

In short, this project is a practical Apple II floating-point toolkit for cc65 developers: it brings Applesoft-compatible floating-point capability to C programs while staying faithful to the Apple II platform's native numeric model.

## Why it exists

The stock cc65 compiler suite does not include native support for C float or double types in the way a modern system does. This project fills that gap for Apple II development by exposing a practical floating-point library that speaks the language of the Apple II runtime environment with a very small memory footprint (less than 512 bytes).

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

The repository contains the library sources, Apple II assembly glue, a build script, test utilities, and generated documentation. The build process compiles the library and packages it into a disk image format used by Apple II emulation and hardware.
