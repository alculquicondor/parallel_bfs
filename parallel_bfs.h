#ifndef DISTRIBUTED_BFS_PARALLEL_BFS_H_
#define DISTRIBUTED_BFS_PARALLEL_BFS_H_

#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives/scatter.hpp>
#include <boost/serialization/vector.hpp>

namespace mpi = boost::mpi;

using std::vector;

class ParallelBFS {
private:
  const mpi::communicator &comm;
  long firstVertex, numVertices;
  vector<long> vertices;
  vector<long> edges;
  long *distance;
public:
  ParallelBFS(const mpi::communicator &comm,
              const vector<long> &vertices,
              const vector<long> &edges);
  ParallelBFS(const mpi::communicator &comm);
  void calculate(int u);
  long size() {
    return vertices.size();
  }
  long getDistance(long u) {
    return distance[u];
  }
  long getFirstVertex() const {
    return firstVertex;
  }
};

#endif //DISTRIBUTED_BFS_PARALLEL_BFS_H_
