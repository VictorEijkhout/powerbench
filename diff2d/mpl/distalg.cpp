/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** distalg.cpp : distributed vector routines
 ****
 ****************************************************************/

#define HAVE_SPAN
#define USE_SPAN

#include <string>
using std::string;
#include <tuple>
using std::pair,std::make_pair,std::tuple,std::make_tuple;
#include <vector>
using std::vector;

#include <iostream>
using std::cout;
#include <format>
using std::format;

#include <ranges>
namespace rng = std::ranges;

#include "distalg.hpp"

#include <memory>
using std::shared_ptr,std::make_shared;

namespace linalg {
  //codesnippet d2ddistributed
  template< typename real >
  distributed_array<real>::distributed_array
      ( const mpl::cartesian_communicator& comm,size_t m,size_t n,bool trace )
        : comm(comm),m_global(m),n_global(n) {
    auto rank = comm.rank();
    /*
     * set up processor grid
     */
    dimensions = comm.get_dimensions();
    auto [pmsize,pnsize] = proc_grid_size();
    proc_start_m = vector<size_t>(pmsize+1);
    proc_start_n = vector<size_t>(pnsize+1);
    /*
     * Fill in processor details
     */
    for ( int pi=0; pi<=pmsize; pi++ )
      proc_start_m.at(pi) = pi*m/pmsize;
    for ( int pi=0; pi<=pnsize; pi++ )
      proc_start_n.at(pi) = pi*n/pnsize;
    if (rank==0 and trace) {
      rng::for_each( proc_start_m,
		     [init=true] ( auto p ) mutable {
		       if (init) { init=false; cout << format("pm: {}",p); } 
		       else cout << format(", {}",p); } ); cout << format("\n");
      rng::for_each( proc_start_n,
		     [init=true] ( auto p ) mutable {
		       if (init) { init=false; cout << format("pn: {}",p); } 
		       else cout << format(", {}",p); } ); cout << format("\n");
    }
    /*
     * This process in particular
     */
    coord = comm.coordinates(rank);
    auto pm = coord[0], pn = coord[1];
    auto
      sm = proc_start_m[pm+1]-proc_start_m[pm],
      sn = proc_start_n[pn+1]-proc_start_n[pn];
    data = bordered_array<real>(sm,sn);
    set_neighbors(trace);
  };
  //codesnippet end

  template< typename real >
  pair<int,int> distributed_array<real>::proc_grid_size() {
    int pm = dimensions.size(0), pn = dimensions.size(1);    
    return make_pair(pm,pn);
  };

  /*! Auxiliary for constructor.
   * We use matrix-numbering: down and right
   */
  //codesnippet d2dnsew
  template< typename real >
  void distributed_array<real>::set_neighbors( bool trace ) {
    mpl::shift_ranks shifted = comm.shift
      ( /* dim: */ 0,/* increasing: */ 1 );
    neighbors['S'] = 
      [shifted] () { auto v = shifted.destination; 
                     return (v<0 ? MPI_PROC_NULL : v); }();
    neighbors['N'] = 
      [shifted] () { auto v = shifted.source; 
                     return (v<0 ? MPI_PROC_NULL : v); }();
    shifted = comm.shift
      ( /* dim: */ 1,/* increasing: */ 1 );
    neighbors['E'] = 
      [shifted] () { auto v = shifted.destination;
                     return (v<0 ? MPI_PROC_NULL : v); }();
    neighbors['W'] = 
      [shifted] () { auto v = shifted.source; 
                     return (v<0 ? MPI_PROC_NULL : v); }();
    //codesnippet end
    if (trace) { 
      cout << format("[{}] ",comm.rank());
      for_each ( neighbors.begin(),neighbors.end(),
                 [init=true] ( auto n ) mutable {
                   if (init) init=false; else cout << format(", ");
                   auto [d,r] = n; cout << format("{}={}",d,r);
                 } );
      cout << format("\n"); }
  };

  //! Derive pointer and layout for the halo, given the source direction
  template< typename real >
  BUFFER distributed_array<real>::get_halo( char direction ) {
    auto [m,n,b] = data.outer_sizes();
    real* ptr; 
    if ( direction=='N' )
      ptr = &( data.data2d()[0,b] );
    else if ( direction=='W' )
      ptr = &( data.data2d()[b,0] );
    else if ( direction=='E' )
      ptr = &( data.data2d()[b,n-b] );
    else if ( direction=='S' )
      ptr = &( data.data2d()[m-b,b] );
    shared_ptr<mpl::layout<real>> layout;
    if (b>1)
      throw( "can not make layout for b>1" );
    if ( direction=='N' or direction=='S' )
      layout = shared_ptr<mpl::layout<real>>
        ( make_shared<mpl::contiguous_layout<real>>(n-2*b) );
    else
      layout = shared_ptr<mpl::layout<real>>
        ( make_shared<mpl::strided_vector_layout<real>>(m-2*b,b,n) );
    return make_pair(ptr,layout);      
  };
  //! Derive pointer and layout for the inner edge, given the target direction
  template< typename real >
  BUFFER distributed_array<real>::get_edge( char direction ) {
    auto [m,n,b] = data.outer_sizes();
    real* ptr; 
    if ( direction=='N' )
      ptr = &( data.data2d()[b,b] );
    else if ( direction=='W' )
      ptr = &( data.data2d()[b,b] );
    else if ( direction=='E' )
      ptr = &( data.data2d()[b,n-2*b] );
    else if ( direction=='S' )
      ptr = &( data.data2d()[m-2*b,b] );
    shared_ptr<mpl::layout<real>> layout;
    if (b>1)
      throw( "can not make layout for b>1" );
    if ( direction=='N' or direction=='S' )
      //codesnippet d2dlythor
      layout = shared_ptr<mpl::layout<real>>
        ( make_shared<mpl::contiguous_layout<real>>(n-2*b) );
      //codesnippet end
    else
      //codesnippet d2dlytver
      layout = shared_ptr<mpl::layout<real>>
        ( make_shared<mpl::strided_vector_layout<real>>(m-2*b,b,n) );
      //codesnippet end
    return make_pair(ptr,layout);      
  };
  //! Halo exchange by loopoing over 4 directions.
  //codesnippet d2dexchange
  template< typename real >
  void distributed_array<real>::halo_exchange( bool trace ) {
    for ( auto [f,t] : { make_pair('W','E'),{'E','W'},{'N','S'},{'S','N'} } ) {
      // inner edge to send in the `to' direction
      auto [snd_ptr,snd_layout] = this->get_edge(t);
      // halo region to receive from the `from' direction
      auto [rcv_ptr,rcv_layout] = this->get_halo(f);
      // neighbor ranks
      auto from = neighbors[f], to = neighbors[t];
      mpl::tag_t t0{0};
      comm.sendrecv
        ( snd_ptr,*snd_layout,to,  t0,
          rcv_ptr,*rcv_layout,from,t0);
    }
  };
  //codesnippet end

  //! Other is by non-const reference because we do halo exchange on it.
  template< typename real >
  void distributed_array<real>::central_difference_from
      ( distributed_array<real>& other,bool trace ) {
    other.halo_exchange( trace );
    data.central_difference_from( other.data );
  };

  /*
   * Logging
   */
  template< typename real >
  void distributed_array<real>::log_flops( float n ) {
    data.log_flops(n); };
  template< typename real >
  void distributed_array<real>::log_bytes( float n ) {
    data.log_bytes(n); };
  template< typename real >
  std::pair<float,float> distributed_array<real>::log_report() {
    // TODO this needs a reduction
    return data.log_report(); };

  //! Gather an array for printing
  template< typename real >
  void distributed_array<real>::view( string caption ) {
    size_t global_size{0}; auto myrank = comm.rank();
    // size without borders
    size_t local_size = data.inner_size();
    // send data is strided vector because of border
    vector<real> internal_data = data.internal_data();
    real* first_data_point = 
      internal_data.data() + 2*data._n2b + 2*data.border;
    auto proc_inner_layout = [*this] ( auto rank ) { 
      auto coord = comm.coordinates(rank);
      auto pm = coord[0], pn = coord[1];
      auto msize = proc_start_m[pm+1]-proc_start_m[pm];
      auto bs    = proc_start_n[pn+1]-proc_start_n[pn];
      return mpl::strided_vector_layout<real>( msize, bs, data._n2b );
    };
    mpl::layout<real> send_layout = proc_inner_layout(comm.rank());
    if ( myrank==0 ) {
      comm.reduce( mpl::plus<size_t>(),0,local_size,global_size );
      vector<real> gathered_data(global_size);
      const auto nprocs = comm.size();
      mpl::layouts<real> recv_layouts(nprocs);
      mpl::displacements recv_displs(nprocs);
      for ( auto iproc=0; iproc<nprocs; iproc++ ) {
	auto coord = comm.coordinates(iproc);
	auto pm = coord[0], pn = coord[1];
	auto msize = proc_start_m[pm+1]-proc_start_m[pm];
	auto bs    = proc_start_n[pn+1]-proc_start_n[pn];
	recv_layouts[iproc] = proc_inner_layout(iproc /* ,n_global+2*data.border ??? */ );
	// mpl::strided_vector_layout<real>( msize, bs, nglobal+2*border );
	recv_displs[iproc] = proc_start_m[pm]*n_global + proc_start_n[pm];
      }
      comm.gatherv
        ( 0,
	  first_data_point,send_layout,
          gathered_data.data(),recv_layouts,recv_displs );
      // view as zero-border array
      bordered_array<real>( m_global,n_global,gathered_data.data() ).view(caption);
    } else {
      comm.reduce( mpl::plus<size_t>(),0,local_size );
      comm.gatherv( 0, first_data_point,send_layout );
    }
  };

  //! Norm computation
  //codesnippet d2dnormmpl
  template< typename real >
  real distributed_array<real>::l2norm() { 
    real local_norm = data.l2norm(), global_norm;
    local_norm *= local_norm;
    comm.allreduce( mpl::plus<real>(),local_norm,global_norm);
    return std::sqrt(global_norm);
  };
  //codesnippet end

  //! Scale the local array
  //codesnippet d2dscalempl
  template< typename real >
  void distributed_array<real>::scale_interior
      ( const distributed_array<real>& other, real factor ) {
    data.scale_interior(other.data,factor);
  };
  //codesnippet end

  template< typename real >
  void distributed_array<real>::copy_interior_from( distributed_array<real> other ) {
    data.copy_interior_from( other.data );
  };
};

namespace linalg {
  template class distributed_array<float>;
  template class distributed_array<double>;
};

