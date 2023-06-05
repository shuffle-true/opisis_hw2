cmake_minimum_required(VERSION 3.0)
project(opisis_hw2 C CXX)

add_subdirectory(third_party/matplotplusplus)

add_executable(run_modeling src/modeling.cc)
target_link_libraries(run_modeling PUBLIC matplot)

add_executable(plot_result src/plotting.cc)
target_link_libraries(plot_result PUBLIC matplot)