#include "parallel_bfs.h"

ParallelBFS::ParallelBFS(const mpi::communicator &comm,
                         const NodeList &vertices,
                         const NodeList &edges) :
    comm(comm) {
  NodeId part_size = (NodeId)vertices.size() / comm.size(),
      left_vertices = (NodeId)vertices.size() % comm.size(),
      first_vertex = 0, first_edge = 0;
  NodeList part_vertices((size_t)comm.size());
  NodeList first_vertices((size_t)comm.size());
  NodeList part_edges((size_t)comm.size());
  NodeList first_edges((size_t)comm.size());
  NodeList all_description((size_t)comm.size() * 3);
  for (int i = 0; i < comm.size(); ++i) {
    NodeId this_part_size = part_size + (i < left_vertices);
    NodeId last_edge = first_vertex + this_part_size == vertices.size() ?
                      (NodeId)edges.size() :
                      vertices[first_vertex + this_part_size];
    all_description[3 * i] = first_vertices[i] = first_vertex;
    all_description[3 * i + 1] = part_vertices[i] = this_part_size;
    all_description[3 * i + 2] = part_edges[i] = last_edge - first_edge;
    first_edges[i] = first_edge;
    first_edge = last_edge;
    first_vertex += this_part_size;
  }
  NodeList description(3);
  mpi::scatter(comm, all_description.data(), description.data(), 3, 0);
  this->first_vertex = description[0];
  mpi::scatterv(comm, vertices, part_vertices, first_vertices,
                this->vertices, description[1], 0);
  mpi::scatterv(comm, edges, part_edges, first_edges,
                this->edges, description[2], 0);
  for (NodeId &v : this->vertices)
    v -= this->vertices[0];
}

ParallelBFS::ParallelBFS(const mpi::communicator &comm) : comm(comm) {
  NodeList description(3);
  mpi::scatter(comm, description.data(), 3, 0);
  first_vertex = description[0];
  mpi::scatterv(comm, vertices, description[1], 0);
  mpi::scatterv(comm, edges, description[2], 0);
  for (NodeId &v : vertices)
    v -= vertices[0];
}

void ParallelBFS::calculate(NodeId u) {
}
