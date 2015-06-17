cmake_minimum_required(VERSION 3.2)
project(distributed_bfs)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Boost 1.58 REQUIRED COMPONENTS mpi serialization)

include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${OpenMP_CXX_FLAGS}")

add_executable(bfs main.cpp parallel_bfs.cpp parallel_bfs.h boost_mpi_extra.h)
target_link_libraries(bfs ${MPI_LIBRARIES} ${Boost_LIBRARIES})

add_executable(gen generator.cpp)