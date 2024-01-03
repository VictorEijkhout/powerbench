#!/bin/bash

D2D=/work2/00434/eijkhout/parallel-programming-private/code/diff2d

rsync ${D2D}/{Make*,*.cpp,*.hpp} .
for d in cxx mpl omp rng span tbb ; do 
    ( 
	mkdir -p $d 
	cd $d
	rsync ${D2D}/$d/{Makefile,*.cpp,*.hpp} .
    )
done

for m in Makefile */Makefile ; do 
    sed -i -e 's?../../makefiles?../makefiles?' $m
done
