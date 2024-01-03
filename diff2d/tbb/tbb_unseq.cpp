#if 0
namespace linalg {
  template< typename real >
  class vector : public std::vector<real> {
  private:
    int _m,_n;
  public:
    vector( int m,int n )
      : _m(m),_n(n),std::vector<float>(m*n) {}
    auto data2d() {
      return md::mdspan( std::vector<float>::data(),md::extents{_m,_n} );
    };
    auto domain() {
      const auto& s = data2d();
      return rng::views::cartesian_product
	( rng::views::iota(0,static_cast<int>( s.extent(0) )),
	  rng::views::iota(0,static_cast<int>( s.extent(1) ))
	  );
    }
    auto inner() {
      const auto s = data2d();
      // we really want cbegin / cend but not yet available
      return rng::views::cartesian_product
	( rng::views::iota(1,static_cast<int>( s.extent(0) )-1),
	  rng::views::iota(1,static_cast<int>( s.extent(1) )-1)
	  );
    };
    void central_difference_from( linalg::vector<real> other ) {
      for_each
	( exec::par_unseq,
	  inner().begin(),inner().end(),
	  [out = data2d(), in = other.data2d()] ( auto idx ) {
	    auto [i,j] = idx;
	    out[i,j] = 4*in[i,j] - in[i-1,j] - in[i+1,j] - in[i,j-1] - in[i,j+1]; }
	  );
    };
    void copy_interior_from( linalg::vector<real> other ) {
      for_each
	( exec::par_unseq,
	  inner().begin(),inner().end(),
	  [out = data2d(),in = other.data2d()] ( auto idx ) {
	    auto [i,j] = idx;
	    out[i,j] = in[i,j]; }
	  );
    };
    void scale_interior( real factor ) {
      for_each
	( exec::par_unseq,
	  inner().begin(),inner().end(),
	  [out = data2d(),factor] ( auto idx ) {
	    auto [i,j] = idx;
	    out[i,j] *= factor; }
	  );
    };
    real l2norm() {
      real sum_of_squares{0};
      for_each
	( exec::par_unseq,
	  inner().begin(),inner().end(),
	  [array = data2d(),&sum_of_squares] ( auto idx ) {
	    auto [i,j] = idx;
	    sum_of_squares += array[i,j] * array[i,j]; }
	  );
      return std::sqrt(sum_of_squares);
    };
    void set_bc() {
      for_each
	( exec::par_unseq,
	  domain().begin(),domain().end(),
	  [this,array = data2d()] ( auto idx ) {
	    auto [i,j] = idx;
	    if ( i==_m-1 or j==_n-1 )
	      array[i,j] = 1.;
	  } );
    };
    void view() {
      for_each
	( exec::par_unseq,
	  domain().begin(),domain().end(),
	  [n=this->_n,array = data2d()] ( auto idx ) {
	    auto [i,j] = idx;
	    cout << array[i,j] << ( j<n-1 ? ' ' : '\n' ); }
	  );
      cout << '\n';
    };
  };
};
#endif
