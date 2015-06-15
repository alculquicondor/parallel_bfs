cmake_minimum_required(VERSION 3.2)
project(distributed_bfs)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(Boost 1.58 REQUIRED COMPONENTS mpi serialization)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${OpenMP_CXX_FLAGS}")

add_executable(bfs main.cpp)
target_link_libraries(bfs ${MPI_LIBRARIES})

add_executable(gen generator.cpp)