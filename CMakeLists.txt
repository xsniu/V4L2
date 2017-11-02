cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(UVCVideo)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")



set(SRC_FILE Src/VideoManager.cpp
             Src/main.cpp)

set(HEAD_FILE Src/VideoManager.h)


set(SRC ${SRC_FILE} ${HEAD_FILE})

find_package(OpenCV REQUIRED)
IF(OpenCV_FOUND)
    message("OpenCV library ${Boost_LIBRARY_DIR}")
ENDIF()

include_directories(${OPENCV_INCLUDE_DIR})
link_directories(${OPENCV_LIBRARY})
link_libraries(${OpenCV_LIBS})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(${PROJECT_NAME} ${SRC})