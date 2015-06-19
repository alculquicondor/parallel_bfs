#include <cstdio>
#include <vector>

#include <boost/mpi/environment.hpp>

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
  bfs->calculate(0);
  for (int i = 0; i < bfs->size(); ++i)
    printf("%d: %d\n", bfs->get_first_vertex() + i, bfs->get_distance(i));
  return 0;
}
