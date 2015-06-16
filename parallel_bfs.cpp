#include "parallel_bfs.h"

const typename ParallelBFS::NodeId ParallelBFS::infinity =
    std::numeric_limits<ParallelBFS::NodeId>::max();

ParallelBFS::ParallelBFS(const mpi::communicator &comm,
                         const NodeList &vertices,
                         const NodeList &edges) :
    comm(comm) {
  NodeId part = (NodeId)vertices.size() / comm.size(),
      left_vertices = (NodeId)vertices.size() % comm.size(),
      first_vertex = 0, first_edge = 0;
  NodeList part_vertices((size_t)comm.size());
  NodeList first_vertices((size_t)comm.size());
  NodeList part_edges((size_t)comm.size());
  NodeList first_edges((size_t)comm.size());
  NodeList all_description((size_t)(comm.size() << 2));
  for (int i = 0; i < comm.size(); ++i) {
    NodeId this_part = part + (i < left_vertices);
    NodeId last_edge = first_vertex + this_part == vertices.size() ?
                      (NodeId)edges.size() :
                      vertices[first_vertex + this_part];
    all_description[(i<<2)] = (NodeId)vertices.size();
    all_description[(i<<2) + 1] = first_vertices[i] = first_vertex;
    all_description[(i<<2) + 2] = part_vertices[i] = this_part;
    all_description[(i<<2) + 3] = part_edges[i] = last_edge - first_edge;
    first_edges[i] = first_edge;
    first_edge = last_edge;
    first_vertex += this_part;
  }
  NodeList description(4);
  mpi::scatter(comm, all_description.data(), description.data(), 4, 0);
  this->vertex_total_count = description[0];
  this->first_vertex = description[1];
  mpi::scatterv(comm, vertices, part_vertices, first_vertices,
                this->vertices, description[2], 0);
  mpi::scatterv(comm, edges, part_edges, first_edges,
                this->edges, description[3], 0);
  prepare();
}

ParallelBFS::ParallelBFS(const mpi::communicator &comm) : comm(comm) {
  NodeList description(4);
  mpi::scatter(comm, description.data(), 4, 0);
  vertex_total_count = description[0];
  first_vertex = description[1];
  mpi::scatterv(comm, vertices, description[2], 0);
  mpi::scatterv(comm, edges, description[3], 0);
  prepare();
}

void ParallelBFS::prepare() {
  vertex_count = (NodeId)vertices.size();
  for (NodeId &v : vertices)
    v -= vertices[0];
  vertices.push_back((NodeId)edges.size());
  _vertex_left_parts = vertex_total_count % comm.size();
  _vertex_left_part_size = comm.rank() < _vertex_left_parts ?
                           vertex_count : vertex_count + 1;
}

int ParallelBFS::find_owner(NodeId u) {
  int total_left = _vertex_left_parts * _vertex_left_part_size;
  if (u < total_left)
    return u / _vertex_left_part_size;
  return _vertex_left_parts + (u - total_left) / (_vertex_left_part_size - 1);
}

void ParallelBFS::calculate(NodeId u) {
  distance.assign((size_t) vertex_count, infinity);
  NodeId level = 1;
  NodeList frontier;
  if (comm.rank() == find_owner(u)) {
    frontier.push_back(u - first_vertex);
    distance[u - first_vertex] = 0;
  }
  std::vector<NodeList> send_buf((size_t)comm.size());
  std::vector<NodeList> recv_buf((size_t)comm.size());
  std::vector<std::vector<NodeList>> t_buf(
      (size_t)omp_get_thread_num(), std::vector<NodeList>((size_t)comm.size()));

  while (mpi::all_reduce(comm, (NodeId)frontier.size(),
                         std::plus<NodeId>()) > 0) {
    // TODO
    ++level;
  }
}

