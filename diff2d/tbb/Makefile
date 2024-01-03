################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2023
####
#### makefile for TBB version of dff2d
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo "    Codes for diff2d in C++"
	@echo "================================================================"
	@echo && echo "Available rules:" && echo

APPS = diff2d
WRONGS = 

MODE = tbb
LANGUAGE = CXX
CATEGORY  = tbb
MODULES = cxxopts fmtlib mdspan rangev3
include ../../makefiles/Make.compile
CPPSTANDARD = 23
EXTRA_INCLUDES = -I${TACC_CXXOPTS_INC} -I${TACC_MDSPAN_INC} -I${TACC_RANGEV3_INC} \
    $$( pkg-config --cflags fmt) \
    $$( pkg-config --cflags tbb)
EXTRA_LIBS = \
    $$( pkg-config --libs fmt ) \
    $$( pkg-config --libs tbb )

diff2d.o : ../main.cpp ../options.cpp ../linalg.hpp ../linalg.cpp

##
## Runs
##
RUNS = 
include ../Make.runs
include  ../../makefiles/Make.omp_run
.PHONY: ${RUNS}

##
## cmake
##
CMAKEFILESDIR = ../../makefiles
CMAKE_CXX_FLAGS = \
    -D PSTL_USE_PARALLEL_POLICIES=0 \
    -D _GLIBCXX_USE_TBB_PAR_BACKEND=0

include ../../makefiles/Make.cmake

##
## Clean
##

include  ../../makefiles/Make.clean
MAKEFILESDIR := ../../${MAKEFILESDIR}
MAKETRACESTRING := "${MAKETRACESTRING} .. .. .."
clean :: codeclean
	@/bin/rm -rf ${APPS} ${WRONGS}
