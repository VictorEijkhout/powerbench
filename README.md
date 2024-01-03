# A Power method Benchmark

This is a simple benchmark for performance on HPC-like codes.
Sample implementations in OpenMP and MPI are given.

This is an open invitation for implementations in other systems.

## Basic design

This is a power method implementation where the matrix is the 
five-point central difference stencil.
Thus you essentially need to write three routines:
matrix-vector product, L2 norm, scale vector.

### File structure

All shared memory implementations use `linalg.hpp` for these routines.

All implementation enforce conformity by including `main.cpp`; 
for this purpose
they also include `options.cpp` for commandline options handling.

### Data structure

All implementations use a `linalg::bordered_array<T>` data structure
defined in `linalg.hpp`. This is constructure with `m,n` parameters
describing the domain, and default `border=1`.
Make sure you get your indexing right that you only operate
on the `m,n` part, but for the matrix-vector product 
possibly read from the border too.

## Building

Building the sample implementations relies heavily on the compiler
infrstructure at TACC. Sorry. I'll try to modularize it.

### Prerequisites

The cxxopts package needs to be installed with its `.pc` file in the `PKG_CONFIG_PATH`.

The mdspan package from kokkos needs to be installed. 
Since this has neither a `.pc` file nor a CMake module,
the variable `TACC_MDSPAN_INC` needs to be set to the location
of the include files.

For the MPL/MPI implementation, `TACC_MPL_INC` needs to be set.

