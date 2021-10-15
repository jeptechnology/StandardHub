project(EchoServer)

add_executable(EchoServer
   Uart.cpp
   main.cpp)

set_property(TARGET EchoServer PROPERTY CXX_STANDARD 17)
