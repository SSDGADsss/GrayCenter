cmake_minimum_required(VERSION 3.10)

project(GrayCenter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(inc ${OpenCV_INCLUDE_DIRS} thirdparty/stb)
add_executable(test test/test.cpp src/GrayCenter.cpp)
add_library(${PROJECT_NAME} src/GrayCenter.cpp)
