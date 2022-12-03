cmake_minimum_required(VERSION 3.16)
project(k_means_clustering)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

set(SOURCE_FILES Point.h Point.cpp Cluster.h Cluster.cpp utils.h utils.cpp kMeansClustering.h kMeansClustering.cpp Points.h Clusters.h)

add_executable(k_means_clustering main.cpp ${SOURCE_FILES})
add_executable(k_means_clustering_soa main_soa.cpp ${SOURCE_FILES})

target_link_libraries(k_means_clustering OpenMP::OpenMP_CXX)
target_link_libraries(k_means_clustering_soa OpenMP::OpenMP_CXX)
