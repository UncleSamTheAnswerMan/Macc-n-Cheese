cmake_minimum_required(VERSION 3.3)
project(MicroAStrings)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    mncScan.cpp
    mncScan.h)

add_executable(Macc-n-Cheese ${SOURCE_FILES})