cmake_minimum_required(VERSION 3.7)
project(StdHubHelloWorld)
set(LIBRARIES_FROM_REFERENCES "")

add_executable(StdHubHelloWorld
	StdHubHelloWorld.cpp)

target_link_libraries(StdHubHelloWorld "${LIBRARIES_FROM_REFERENCES}")

set_property(TARGET StdHubHelloWorld PROPERTY CXX_STANDARD 14)
