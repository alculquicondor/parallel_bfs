#ifndef DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_
#define DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_

#include <vector>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/status.hpp>

namespace boost {
namespace mpi {

template<typename T>
void scatterv(const communicator &comm, const std::vector<T> &in_values,
              const std::vector<int> &send_counts,
              const std::vector<int> &displacements,
              std::vector<T> &out_values, int root) {
  MPI_Datatype data_type = mpi::get_mpi_datatype(T());
  MPI_Scatterv(in_values.data(), send_counts.data(), displacements.data(),
               data_type, out_values.data(), (int)out_values.size(), data_type,
               root, comm);
}

template<typename T>
void scatterv(const communicator &comm, std::vector<T> &out_values, int root) {
  MPI_Datatype data_type = mpi::get_mpi_datatype(T());
  MPI_Scatterv(nullptr, nullptr, nullptr, data_type, out_values.data(),
               (int)out_values.size(), data_type, root, comm);
}

template<typename Ts, typename Tr>
status sendrecv(const communicator &comm,
              int dest, int send_tag, const std::vector<Ts> &send_data,
              int source, int recv_tag, std::vector<Tr> &recv_data) {
  MPI_Datatype data_type_send = mpi::get_mpi_datatype(Ts());
  MPI_Datatype data_type_recv = mpi::get_mpi_datatype(Tr());
  MPI_Status _status;
  MPI_Sendrecv(send_data.data(), (int)send_data.size(), data_type_send,
               dest, send_tag, recv_data.data(), (int)recv_data.size(),
               data_type_recv, source, recv_tag, comm, &_status);
  return status(_status);
}

}
}

#endif //DISTRIBUTED_BFS_BOOST_MPI_EXTRA_H_
