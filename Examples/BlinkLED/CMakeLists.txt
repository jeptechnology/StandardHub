cmake_minimum_required(VERSION 3.7)

project(BlinkLED)

add_executable(BlinkLED
   Led.cpp
   main.cpp)

set_property(TARGET BlinkLED PROPERTY CXX_STANDARD 14)
