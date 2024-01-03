################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2023
####
#### makefile for sequential C++ programs
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo "    Sequential codes for diff2d in C++"
	@echo "================================================================"
include ../../makefiles/Make.info

APPS = diff2d
WRONGS = 

MODE = seq
LANGUAGE = CXX
CATEGORY = cxx
MODULES = cxxopts fmtlib mdspan 

##
## dependencies
##
EXTRA_OBJS = linalg.o
diff2d.o : ../main.cpp ../options.cpp ../linalg.hpp
linalg.o : ../linalg.hpp linalg.cpp
## we use the omp implementation, stripping out the omp stuff
linalg.cpp : ../omp/linalg.cpp
	@echo " .. copying $<"
	@cat $< \
	| grep -v pragma \
	| sed -e '/snippet/s/omp/cxx/' \
	> $@

##
## compilation
##
include ../../makefiles/Make.compile
ifeq "${TACC_FAMILY_COMPILER}" "intel"
  CPPSTANDARD = 2b
else
  CPPSTANDARD = 23
endif
EXTRA_INCLUDES = -I${TACC_MDSPAN_INC} \
    $$( pkg-config --cflags cxxopts ) \
    $$( pkg-config --cflags fmt )
EXTRA_LIBS = \
    $$( pkg-config --libs fmt )
RPATH = ${TACC_FMTLIB_LIB}

##
## profiling
##
include ../Make.prof

##
## Runs
##

RUNS = 
RUNS += run_default
include ../Make.runs
include ../../makefiles/Make.omp_run

.PHONY: ${RUNS}

##
## cmake
##

CMAKEFILESDIR = ../../makefiles
CMAKE_CXX_FLAGS = \
include ../../makefiles/Make.cmake

##
## Clean
##

include  ../../makefiles/Make.clean
MAKEFILESDIR := ../../${MAKEFILESDIR}
MAKETRACESTRING := "${MAKETRACESTRING} .. .. .."
clean :: codeclean
	@/bin/rm -rf ${APPS} ${WRONGS}
