project(HelloWorld)

set(LIBRARIES_FROM_REFERENCES "")

add_executable(HelloWorld main.cpp)

target_link_libraries(HelloWorld "${LIBRARIES_FROM_REFERENCES}")

set_property(TARGET HelloWorld PROPERTY CXX_STANDARD 17)
