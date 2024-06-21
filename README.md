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
in Computer Science at _Ca' Foscari_ University of Venice. Apart from
my PhD thesis, this library has been described in the following
publication (see [CITATION](CITATION) for a BibTeX format):

> Moreno Marzolla, _libcppsim: a Simula-like, Portable
> Process-Oriented Simulation Library in C++_, Proceedings of ESM'04,
> the 18th European Simulation Multiconference (Graham Horton,
> editor), Magdeburg, DE, jun 13–16 2004, ISBN 3-936150-35-4,
> pp. 222–227
> (https://www.moreno.marzolla.name/publications/papers/libcppsim-a-Simula-like-Portable-Process-Oriented-Simulation-Library.pdf)[PDF]

This library has not been updated since a long time, and does not use
any feature of "modern" C++ (which might even be considered a good
thing). However, there are people around the world still using it, so
I thought it would be a good idea to make it available on this
repository.

## Compilation

The compilation is currently based on make/autoconf:

    ./configure
    make
