#ifndef DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_
#define DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_

#include <vector>
#include <boost/mpi/communicator.hpp>

namespace boost {
namespace mpi {

template<typename T>
void scatterv(const communicator &comm, const std::vector<T> &in_values,
              const std::vector<int> &send_counts,
              const std::vector<int> &displacements,
              std::vector<T> &out_values, int n, int root) {
  MPI_Datatype data_type = mpi::get_mpi_datatype(T());
  out_values.resize((size_t)n);
  MPI_Scatterv(in_values.data(), send_counts.data(), displacements.data(),
               data_type, out_values.data(), n, data_type, root, comm);
}

template<typename T>
void scatterv(const communicator &comm, std::vector<T> &out_values, int n,
              int root) {
  MPI_Datatype data_type = mpi::get_mpi_datatype(T());
  out_values.resize((size_t)n);
  MPI_Scatterv(nullptr, nullptr, nullptr, data_type, out_values.data(),
               n, data_type, root, comm);
}

}
}

#endif //DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_
