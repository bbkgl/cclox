cmake_minimum_required(VERSION 3.16)
project(cclox)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/cclox)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/cclox/GarbageCollect DIR_GC_SRCS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/cclox DIR_SRCS)

add_executable(cclox main.cpp ${DIR_SRCS} ${DIR_GC_SRCS})
