/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2024 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** diff2d.cpp : 2D diffusion in parallel using ranges, whatever that means
 ****
 ****************************************************************/

#include <chrono>

#include <algorithm>
using std::for_each;

#include <cassert>

#include <iostream>
using std::cout;
#include <format>
using std::format;

#include "cxxopts.hpp"

#define HAVE_SPAN
#include "../linalg.hpp"

using real = float;

int main(int argc,char **argv) {

#include "../options.cpp"
  const int procno{0};
  const std::string prefix{"rng"};
  if (result.count("Help")) {
    std::cout << "================ This type of run:\n";
    std::cout << "OpenMP 1D parallel loop\n";
    std::cout << "of i,j pairs ranging over inner\n";
    return 0;
  }

  if (trace)
    cout << format("Vector size: {} x {}\n",msize,nsize);
  linalg::bordered_array<real> X(msize,nsize),Y(msize,nsize);
#include "../main.cpp"

  return 0;
}
