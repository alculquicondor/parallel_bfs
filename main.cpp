#include <cstdio>
#include <vector>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/mpi/collectives/reduce.hpp>

#include "parallel_bfs.h"

int main(int argc, char *argv[1]) {
  mpi::environment env(argc, argv);
  mpi::communicator world;

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

  mpi::timer timer;
  bfs->calculate(0);
  double elapsed = timer.elapsed(), global_elapsed;
  mpi::reduce(world, elapsed, global_elapsed, mpi::minimum<double>(), 0);
  if (world.rank() == 0)
    fprintf(stderr, "Running time: %lf s\n", global_elapsed);

  for (int i = 0; i < bfs->size(); ++i)
    printf("%d: %d\n", bfs->get_first_vertex() + i, bfs->get_distance(i));
  delete bfs;
  return 0;
}
