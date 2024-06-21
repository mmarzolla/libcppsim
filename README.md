# libcppsim

## Description

`libcppsim` is a process-oriented, discrete-event simulation library
for the C++ programming language. `libcppsim` provides the user with a
set of basic primitives which are roughly equivalent to those provided
by the `SIMULATION` class of the Simula language. `libcppsim` defines
a _process_ class which represents a simulation process. Moreover,
facilities for process scheduling and statistics collection are also
provided. The user may build more sophisticated simulation entities or
synchronization mechanisms on top of those provided.

`libcppsim` was originally developed by Moreno Marzolla during his PhD
in Computer Science at _Ca' Foscari_ University of Venice. This
library has not been updated since a long time; however, it is written
in standard C++ without any "modern" feature (which might even be
considered a good thing), and therefore still compiles on modern Linux
distributions (tested with Ubuntu 20.04). Since there are people
around the world still using it, I made the source code available on
this repository.

## Requirements

This program has been tested on Linux, but should work on other
Unix-like systems. The only system-dependent file is the
implementation of
[coroutines](https://en.wikipedia.org/wiki/Coroutine) that requires
either `setjmp/longjmp` or `makecontext/swapcontext` system calls.
The coroutine implementation used in the `libcppsim` library is based
on

> K. Helsgaun, _A portable C++ library for coroutine sequencing_,
> DATALOGISKE SKRIFTER (Writings on Computer Science), 1999, Roskilde
> University.

Details and source code are available
[here](http://akira.ruc.dk/~keld/research/COROUTINE/).

## Compilation

The compilation is currently based on make/autoconf:

    ./configure
    make

## Citation

The `libcppsim` library has been described in the following
publication (see [CITATION](CITATION) for a BibTeX format):

> Moreno Marzolla, _libcppsim: a Simula-like, Portable
> Process-Oriented Simulation Library in C++_, Proceedings of ESM'04,
> the 18th European Simulation Multiconference (Graham Horton,
> editor), Magdeburg, DE, jun 13–16 2004, ISBN 3-936150-35-4,
> pp. 222–227
> [PDF](https://www.moreno.marzolla.name/publications/papers/libcppsim-a-Simula-like-Portable-Process-Oriented-Simulation-Library.pdf)
