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

## Compilation

The compilation is currently based on make/autoconf:

    ./configure
    make
