cmake_minimum_required(VERSION 3.3)
project(Macc-N-Cheese)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mncScan.cpp
    mncScan.h "Scanner Test/mncScanTest.cpp" mncCode.cpp mncCode.h main.cpp)

add_executable(Macc-n-Cheese ${SOURCE_FILES})