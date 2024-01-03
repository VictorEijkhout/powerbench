/****************************************************************
 ****
 **** This file belongs with the course
 **** Parallel Programming in MPI and OpenMP
 **** copyright 2019-2023 Victor Eijkhout eijkhout@tacc.utexas.edu
 ****
 **** main.cpp : general main for diff2d codes
 ****
 ****************************************************************/

cxxopts::Options options
("diff2d",
 "================================\nPrime numbers\n================================");
options.add_options()
("h,help","usage information")
("H,Help","elaborate information about this type of run")
("m,msize","size in 1st direction",cxxopts::value<int>()->default_value("10"))
("n,nsize","size in 2nd direction",cxxopts::value<int>()->default_value("10"))
("b,border","border around omega",cxxopts::value<int>()->default_value("1"))
("i,itcount","iteration count",cxxopts::value<int>()->default_value("10"))
#ifdef USE_OMP
("c,collapse","loop collapse",cxxopts::value<int>()->default_value("2"))
#endif
#ifdef USE_TBB
("p,parallelism","number of threads",cxxopts::value<int>()->default_value("4"))
#endif
#ifdef USE_MPI
("p,parallelism","number of processes",cxxopts::value<int>()->default_value("4"))
#endif
("t,trace","view arrays",cxxopts::value<bool>()->default_value("false"))
("v,view","view arrays",cxxopts::value<bool>()->default_value("false"))
;

auto result = options.parse(argc,argv);
if (result.count("help")) {
  std::cout << options.help() << '\n';
  return 1;
 }
int itcount  = result["i"].as<int>();

int border   = result["b"].as<int>();
int msize    = result["m"].as<int>();
int nsize    = result["n"].as<int>();
#ifdef USE_OMP
int collapse = result["c"].as<int>();
#endif

bool trace   = result["t"].as<bool>();
bool view    = result["v"].as<bool>();
bool help_msg = result.count("Help")>0;
