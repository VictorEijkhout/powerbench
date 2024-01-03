/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** diff2d.cpp : 2D diffusion sequentially
 ****
 ****************************************************************/

#undef USE_TBB

#include <chrono>
#include <iostream>
using std::cin;
using std::cout;
#include <cassert>

#include "../linalg.hpp"

#include "cxxopts.hpp"
#include "fmt/format.h"
using fmt::print;

using real = float;

int main(int argc,char **argv) {

#include "../options.cpp"
  const int procno{0};
  const std::string prefix{"cxx"};
  if (result.count("Help")) {
    std::cout << "================ This type of run:\n";
    std::cout << "Sequential run from striping OpenMP version\n";
    return 0;
  }

  if (trace)
    print("Vector size: {} x {}\n",msize,nsize);
  linalg::bordered_array<real> X(msize,nsize),Y(msize,nsize);
#include "../main.cpp"

  return 0;
}
