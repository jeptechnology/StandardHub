cmake_minimum_required(VERSION 3.7)

project(ColourCycle)

add_executable(ColourCycle
   Led.cpp
   Button.cpp
   main.cpp)

set_property(TARGET ColourCycle PROPERTY CXX_STANDARD 14)
