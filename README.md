# Prerequisites

The cxxopts package needs to be installed with its `.pc` file in the `PKG_CONFIG_PATH`.

The mdspan package from kokkos needs to be installed. 
Since this has neither a `.pc` file nor a CMake module,
the variable `TACC_MDSPAN_INC` needs to be set to the location
of the include files.

