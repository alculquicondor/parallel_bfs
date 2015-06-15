#include <cstdio>
#include <vector>

#include <omp.h>

#include <boost/mpi/environment.hpp>

#include "parallel_bfs.h"

int main(int argc, char *argv[1]) {
  mpi::environment env;
  mpi::communicator world;
  omp_set_num_threads(argc > 1 ? atoi(argv[1]) : 2);

  ParallelBFS *bfs;
  if (world.rank() == 0) {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<long> vertices(n), edges(m);
    for (long &x : vertices)
      scanf("%ld", &x);
    for (long &x : edges)
      scanf("%ld", &x);
    bfs = new ParallelBFS(world, vertices, edges);
  } else {
    bfs = new ParallelBFS(world);
  }
  bfs->calculate(0);
  return 0;
}
