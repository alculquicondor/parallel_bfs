#include <cstdio>
#include <vector>

#include <boost/mpi/environment.hpp>

#include "parallel_bfs.h"

int main(int argc, char *argv[1]) {
  mpi::environment env(argc, argv);
  mpi::communicator world;
  omp_set_num_threads(argc > 1 ? atoi(argv[1]) : 2);

  ParallelBFS *bfs;
  if (world.rank() == 0) {
    int n, m;
    scanf("%d %d", &n, &m);
    ParallelBFS::NodeList vertices((size_t)n), edges((size_t)m);
    for (ParallelBFS::NodeId &x : vertices)
      scanf("%d", &x);
    for (ParallelBFS::NodeId &x : edges)
      scanf("%d", &x);
    bfs = new ParallelBFS(world, vertices, edges);
  } else {
    bfs = new ParallelBFS(world);
  }
  bfs->calculate(0);
  return 0;
}
