################################################################
################################################################
####
#### This makefile is part of the source of 
#### "Parallel Computing for Science and Engineering"
#### by Victor Eijkhout, copyright 2013-2023
####
################################################################
################################################################

.PHONY: info
info ::

VARIANTS = cxx omp clps rng span
# tile
.PHONY: compile
compile :
	@for v in ${VARIANTS} ; do \
	    ( echo "Making variant $${v}" \
	       && cd $${v} \
	       && make --no-print-directory diff2d \
	    ) \
	; done
.PHONY: run_scaling
info ::
	@echo "run_scaling : big run across all variants"
	@echo "    [ NSIZE= (default: ${NSIZE}) ]"
NSIZE = 30000
run_scaling :
	@for v in ${VARIANTS} ; do \
	  ( cd $${v} \
	     && make --no-print-directory run_scaling \
	          NDIV=10 NSIZE=${NSIZE} ECHO=1 \
	  ) \
	 ; done

include ../makefiles/Make.clean
clean :: recursive_clean
MAKEFILESDIR := ../${MAKEFILESDIR}
MAKETRACESTRING := "${MAKETRACESTRING} .. .."
