cmake_minimum_required(VERSION 3.7)

project(HelloWorld)

set(LIBRARIES_FROM_REFERENCES "")

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld "${LIBRARIES_FROM_REFERENCES}")

set_property(TARGET HelloWorld PROPERTY CXX_STANDARD 14)
