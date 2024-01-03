/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** main.cpp : general main for diff2d codes
 ****
 ****************************************************************/

X.set( 1.,trace );
if (view)
  X.view("Original");

auto xnorm = X.l2norm();
if ( trace and procno==0 )
  print("x norm: {}\n",xnorm);

using myclock = std::chrono::steady_clock;
auto start_time = myclock::now();

float fl{0},bw{0};
//codesnippet d2dmain
for ( int it=0; it<itcount; it++ ) {
  /*
   * Matrix-vector product
   */
  Y.central_difference_from(X,trace);
  bw += 5.*msize*nsize; fl += 5*msize*nsize; //snippetskip
  if (view)
    Y.view("Operator applied");
  // norm computation
  auto bnorm = Y.l2norm();
  bw += 2.*msize*nsize; fl += msize*nsize; //snippetskip
  if ( trace and procno==0 )
    print("[{:>2}] y norm: {}\n",it,bnorm);
  // scale
  X.scale_interior( Y,1./bnorm );
  bw += 1.*msize*nsize; fl += msize*nsize; //snippetskip
  if ( view )
    X.view("Scaled");
 }
//codesnippet end

/*
 * Time reporting
 */
auto duration = myclock::now()-start_time;
auto millisec_duration = 
  std::chrono::duration_cast<std::chrono::microseconds>(duration)/1000;
auto msec = millisec_duration.count();
if ( procno==0 )
  print("Time: {:>6} msec\n",msec);

/*
 * Other stats
 */
if ( procno==0 )
  print("{}-BW: (estimated) {:7.3}\n",
	prefix,bw/(msec/1.e3)*1.e-9);
if ( procno==0 )
  print("{}-Flops: {:7.3}\n",
	prefix,fl/(msec/1.e3)*1.e-9);

// auto [f,b] = X.log_report();
// if ( procno==0 )
//   print("Gflop rate: {:5.2}, Gbyte rate: {:5.2}\n",f,b);
