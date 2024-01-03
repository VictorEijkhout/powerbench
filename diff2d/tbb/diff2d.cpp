/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** diff2d.cpp : 2D diffision in parallel through policies
 ****
 ****************************************************************/

#include <iostream>
using std::cin;
using std::cout;

#include <algorithm>
using std::for_each;
#include <execution>
namespace exec = std::execution;

#include <numeric>
using std::reduce;
#include <tuple>
using std::tuple;
#include <vector>
using std::vector;

#include <cassert>

//#include <ranges>
#include <range/v3/all.hpp>
#ifdef RANGES_V3_ALL_HPP
namespace rng = ranges;
#else
namespace rng = std::ranges;
#endif

// #include <mdspan>

#include "mdspan/mdspan.hpp"
namespace md = Kokkos;
namespace KokkosEx = MDSPAN_IMPL_STANDARD_NAMESPACE::MDSPAN_IMPL_PROPOSED_NAMESPACE;
namespace mdx = KokkosEx;

#define USE_TBB
#include "tbb/tbb.h"

#include "cxxopts.hpp"
#include "../linalg.cpp"
#include "fmt/format.h"
using fmt::print;

using real = float;

int main(int argc,char **argv) {

#include "../options.cpp"

  int nthreads = result["p"].as<int>();
  tbb::global_control ctl(tbb::global_control::max_allowed_parallelism, nthreads);
  const std::string prefix{"tbb"};
  const int procno{0};

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
