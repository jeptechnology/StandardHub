cmake_minimum_required(VERSION 3.7)

project(EchoServer)

add_executable(EchoServer
   Uart.cpp
   main.cpp)

set_property(TARGET EchoServer PROPERTY CXX_STANDARD 14)
