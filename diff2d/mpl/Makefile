################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2023
####
#### This is the makefile for the diff2d using MPL
####
################################################################
################################################################

info ::
	@echo "================================================================"
	@echo "    MPL/MPI codes for diff2d in C++"
	@echo "================================================================"
include ../../makefiles/Make.info

APPS = diff2d
WRONGS = 

MODE = mpl
LANGUAGE = CXX
CATEGORY = mpl
MODULES = cxxopts fmtlib mdspan rangev3 mpl
.SECONDEXPANSION:
EXTRA_OBJS = distalg.o linalg.o
include ../../makefiles/Make.compile
ifeq "${TACC_FAMILY_COMPILER}" "intel"
  CPPSTANDARD = 2b
else
  CPPSTANDARD = 23
endif
EXTRA_INCLUDES = -I${TACC_MDSPAN_INC} -I${TACC_RANGEV3_INC} \
    $$( pkg-config --cflags fmt ) \
    $$( pkg-config --cflags cxxopts )
EXTRA_LIBS = \
    $$( pkg-config --libs fmt )

##
## get the sequential basic routines
##
linalg.o : ../omp/linalg.cpp ../linalg.hpp
	@cd ../cxx && make linalg.o ECHO=${ECHO}
	@cp ../cxx/linalg.o .
## other dependencies
distalg.o : ../linalg.hpp ./distalg.hpp
diff2d.o : ../main.cpp ../options.cpp ./distalg.hpp

RUNS =
info ::
	@echo "make run_XXX (in ${RUNS})"
include ../Make.runs
include ../../makefiles/Make.mpi_run

include ../../makefiles/Make.clean
CATCHER = ../../../teaching/course-support/catchout

clean ::
	@rm -f *.dat
