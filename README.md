# A Power method Benchmark

This is a simple benchmark for performance on HPC-like codes.
Sample implementations in OpenMP and MPI are given.

This is an open invitation for implementations in other systems.

## Prerequisites

The cxxopts package needs to be installed with its `.pc` file in the `PKG_CONFIG_PATH`.

The mdspan package from kokkos needs to be installed. 
Since this has neither a `.pc` file nor a CMake module,
the variable `TACC_MDSPAN_INC` needs to be set to the location
of the include files.

For the MPL/MPI implementation, `TACC_MPL_INC` needs to be set.

## Building

Building the sample implementations relies heavily on the compiler
infrstructure at TACC. Sorry. I'll try to modularize it.
