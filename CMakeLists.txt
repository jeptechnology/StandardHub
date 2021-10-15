# New in version 3.19: An interface library target may be created with source files
# This allows the GitVersion.gd to be a proper dependency
cmake_minimum_required(VERSION 3.16)

# Code generation steps
if (WIN32)
   set(GNUMAKE ${CMAKE_SOURCE_DIR}/tools/GNU/make.exe)
else ()
   set(GNUMAKE make)
endif ()

project(StandardHub VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(CMakeDependentOption)

message(STATUS "Cross Compiling...${CMAKE_CROSSCOMPILING}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FETCHCONTENT_QUIET OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -ggdb -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -ggdb -ffunction-sections -Wno-psabi")

message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -ftree-coalesce-vars")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -ftree-coalesce-vars")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

message("CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
add_definitions(-D__linux__ -D__gnu_linux__ -D_GNU_SOURCE -D_POSIX_SOURCE)

add_subdirectory(Examples)

# file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Stripped)
# add_custom_command(TARGET WiserHomeLinux POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/WiserHomeLinux ${CMAKE_CURRENT_BINARY_DIR}/Stripped/WiserHomeLinux
#    COMMAND ${TOOLCHAIN_ROOT}/bin/armv7l-linux-musleabihf-strip${exe} ${CMAKE_CURRENT_BINARY_DIR}/Stripped/WiserHomeLinux
#    COMMAND ${CMAKE_COMMAND} -E echo "SUCCESS: WiserHomeLinux is now ready for emplacing in a docker container"
#    COMMENT Stripping WiserHomeLinux application binary of debugging symbols...
# )
