#ifndef DISTRIBUTED_BFS_PARALLEL_BFS_H_
#define DISTRIBUTED_BFS_PARALLEL_BFS_H_

#include <boost/mpi/collectives/scatter.hpp>
#include <boost/serialization/vector.hpp>

#include "boost_mpi_extra.h"

namespace mpi = boost::mpi;

class ParallelBFS {
public:
  typedef int NodeId;
  typedef std::vector<NodeId> NodeList;
private:
  const mpi::communicator &comm;
  NodeId first_vertex;
  NodeList vertices;
  NodeList edges;
  NodeList distance;
public:
  ParallelBFS(const mpi::communicator &comm,
              const NodeList &vertices,
              const NodeList &edges);
  ParallelBFS(const mpi::communicator &comm);
  void calculate(NodeId u);
  NodeId size() {
    return (NodeId)vertices.size();
  }
  NodeId get_distance(long u) {
    return distance[u];
  }
  NodeId get_first_vertex() const {
    return first_vertex;
  }
};

#endif //DISTRIBUTED_BFS_PARALLEL_BFS_H_
