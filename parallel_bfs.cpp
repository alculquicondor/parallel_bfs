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
  this->vertices.resize((size_t)description[2]);
  mpi::scatterv(comm, vertices, part_vertices, first_vertices,
                this->vertices, 0);
  this->edges.resize((size_t)description[3]);
  mpi::scatterv(comm, edges, part_edges, first_edges,
                this->edges, 0);
  prepare();
}

ParallelBFS::ParallelBFS(const mpi::communicator &comm) : comm(comm) {
  NodeList description(4);
  mpi::scatter(comm, description.data(), 4, 0);
  vertex_total_count = description[0];
  first_vertex = description[1];
  vertices.resize((size_t)description[2]);
  mpi::scatterv(comm, vertices, 0);
  edges.resize((size_t)description[3]);
  mpi::scatterv(comm, edges, 0);
  prepare();
}

void ParallelBFS::prepare() {
  vertex_count = (NodeId)vertices.size();
  NodeId t = vertices[0];
  for (NodeId &v : vertices)
    v -= t;
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

void ParallelBFS::calculate(NodeId root) {
  distance.assign((size_t) vertex_count, infinity);
  NodeId level = 1;
  NodeList frontier;
  if (comm.rank() == find_owner(root)) {
    frontier.push_back(root - first_vertex);
    distance[root - first_vertex] = 0;
  }
  std::vector<NodeList> send_buf((size_t)comm.size());
  NodeList recv_buf((size_t)vertex_count+1);
  NodeList new_frontier;

  while (mpi::all_reduce(comm, (NodeId)frontier.size(),
                         std::plus<NodeId>()) > 0) {
    for (NodeId u : frontier)
      for (int e = vertices[u]; e < vertices[u + 1]; ++e) {
        int v = edges[e];
        send_buf[find_owner(v)].push_back(v);
      }
    new_frontier = send_buf[comm.rank()];
    for (int i = 1; i < comm.size(); ++i) {
      int dest = (comm.rank() + i) % comm.size();
      NodeList &this_send_buf = send_buf[dest];
      std::sort(this_send_buf.begin(), this_send_buf.end());
      this_send_buf.resize(
          std::unique(this_send_buf.begin(), this_send_buf.end())
          - this_send_buf.begin());
      mpi::status status = mpi::sendrecv(
          comm, dest, 0, send_buf[dest], mpi::any_source, 0, recv_buf);
      new_frontier.insert(new_frontier.end(), recv_buf.begin(),
                          recv_buf.begin() + status.count<NodeId>().get());
    }
    for (size_t i = 0; i < comm.size(); ++i)
      send_buf[i].clear();
    frontier.clear();
    for (int v : new_frontier) {
      v -= first_vertex;
      if (distance[v] == infinity) {
        distance[v] = level;
        frontier.push_back(v);
      }
    }
    ++level;
  }
}

