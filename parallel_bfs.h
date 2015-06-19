#ifndef DISTRIBUTED_BFS_PARALLEL_BFS_H_
#define DISTRIBUTED_BFS_PARALLEL_BFS_H_

#include <limits>
#include <boost/mpi/collectives/scatter.hpp>
#include <boost/mpi/collectives/all_reduce.hpp>
#include <boost/serialization/vector.hpp>

#include "boost_mpi_extra.h"

namespace mpi = boost::mpi;

class ParallelBFS {
public:
  typedef int NodeId;
  typedef std::vector<NodeId> NodeList;
  static const NodeId infinity;
private:
  const mpi::communicator &comm;
  NodeId first_vertex;
  NodeId vertex_count;
  NodeId vertex_total_count;
  NodeId _vertex_left_parts;
  NodeId _vertex_left_part_size;
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
    return vertex_count;
  }
  NodeId get_distance(NodeId u) {
    return distance[u];
  }
  NodeId get_first_vertex() const {
    return first_vertex;
  }
private:
  void prepare();
  int find_owner(NodeId u);
};

#endif //DISTRIBUTED_BFS_PARALLEL_BFS_H_
