#include <iostream>
#include <sstream>

#include <boost/mpi/environment.hpp>
#include <boost/mpi/timer.hpp>
#include <boost/mpi/collectives/reduce.hpp>

#include "parallel_bfs.h"

std::string readLine() {
  std::string s;
  do {
    getline(std::cin, s);
  } while (s.size() && s[0] == '%');
  return s;
}

int main(int argc, char *argv[1]) {
  mpi::environment env(argc, argv);
  mpi::communicator world;

  ParallelBFS *bfs;
  if (world.rank() == 0) {
    std::ios::sync_with_stdio(false);
    int n, m;
    std::stringstream quantities(readLine());
    quantities >> n >> m;
    ParallelBFS::NodeList vertices((size_t)n), edges;
    ParallelBFS::NodeId vertex;
    for (ParallelBFS::NodeId &x : vertices) {
      x = (ParallelBFS::NodeId)edges.size();
      std::stringstream adjacencies(readLine());
      while (adjacencies >> vertex)
        edges.push_back(vertex - 1);
    }
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
