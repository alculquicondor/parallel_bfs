#include "parallel_bfs.h"

ParallelBFS::ParallelBFS(const mpi::communicator &comm,
                         const vector<long> &vertices,
                         const vector<long> &edges) :
    comm(comm) {
  long partSize = vertices.size() / comm.size(),
      leftVertices = vertices.size() % comm.size(), firstVertex = 0;
  vector<long> firstVertices(comm.size());
  vector<vector<long>> verticesParts(comm.size());
  vector<vector<long>> edgesParts(comm.size());
  for (int i = 0; i < comm.size(); ++i) {
    long thisPartSize = partSize;
    if (leftVertices > 0) {
      ++thisPartSize;
      --leftVertices;
    }
    long lastEdgePart = firstVertex + thisPartSize == vertices.size() ?
        edges.size() : vertices[firstVertex+thisPartSize];
    firstVertices[i] = firstVertex;
    verticesParts[i].assign(vertices.begin() + firstVertex,
                            vertices.begin() + firstVertex + thisPartSize);
    edgesParts[i].assign(edges.begin() + vertices[firstVertex],
                         edges.begin() + lastEdgePart);
    firstVertex += thisPartSize;
  }
  mpi::scatter(comm, firstVertices, this->firstVertex, 0);
  mpi::scatter(comm, verticesParts, this->vertices, 0);
  mpi::scatter(comm, edgesParts, this->edges, 0);
}

ParallelBFS::ParallelBFS(const mpi::communicator &comm) : comm(comm) {
  mpi::scatter(comm, firstVertex, 0);
  mpi::scatter(comm, vertices, 0);
  mpi::scatter(comm, edges, 0);
  numVertices = vertices.size();
  for (long &x : vertices)
    x -= vertices[0];
  vertices.push_back(edges.size());
}

void ParallelBFS::calculate(int u) {
}
