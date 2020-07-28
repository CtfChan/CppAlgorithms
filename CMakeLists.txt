cmake_minimum_required(VERSION 3.1)

project (CppAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)



add_executable(bellman_ford 
    graph/BellmanFord.cpp
)