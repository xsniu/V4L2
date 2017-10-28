cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(UVCVideo)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_FLAGS "$CMAKE_CXX_FLAGS -std=c++11 -O0")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SRC_FILE Src/Video.cpp)

set(HEAD_FILE Src/Video.h)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
add_executable(${PROJECT_NAME} ${SRC_FILE} ${HEAD_FILE})