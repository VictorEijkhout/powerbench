/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** diff2d.cpp : 2D diffusion in parallel through MPL
 ****
 ****************************************************************/

#define USE_MPI

#include <chrono>
#include <iostream>
using std::cin;
using std::cout;
#include <map>
using std::map;
#include <cassert>

using real = float;
#include "cxxopts.hpp"
#include "./distalg.hpp"

#include <fmt/format.h>
using fmt::print;
#include <mpl/mpl.hpp>

int main(int argc,char **argv) {

  const std::string prefix{"mpl"};

  const mpl::communicator &comm_world=mpl::environment::comm_world();
  auto procno = comm_world.rank();
  auto nprocs = comm_world.size();

#include "../options.cpp"
  comm_world.bcast(0,itcount);
  comm_world.bcast(0,border);
  comm_world.bcast(0,msize);
  comm_world.bcast(0,nsize);
  comm_world.bcast(0,trace);
  comm_world.bcast(0,view);
  comm_world.bcast(0,help_msg);

  if ( help_msg ) {
    if ( procno==0 ) {
      std::cout << "================ This type of run:\n";
      std::cout << "MPL/MPI run\n";
    }
    return 0;
  }

  if ( trace and procno==0 )
    print("Number of processes: {}\n",nprocs);
  //codesnippet d2ddimscreate
  mpl::cartesian_communicator::dimensions brick(2);
  brick = mpl::dims_create(nprocs,brick);
  mpl::cartesian_communicator cart_comm( comm_world,brick );
  //codesnippet end
  if ( trace and procno==0 )
    print("Process grid: {}x{}\n",brick.size(0),brick.size(1));

  if ( trace and procno==0 )
    print("Setting up domain of {} x {}\n",msize,nsize);
  linalg::distributed_array<real>
    X(cart_comm,msize,nsize,trace), Y(cart_comm,msize,nsize);

  #include "../main.cpp"

  return 0;
}
