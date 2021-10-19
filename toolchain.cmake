# Explicitly setting the cmake system name sets CMAKE_CROSS_COMPILING flag
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv7l)

set(TOOLCHAIN_ROOT "/opt/armv7l-linux-musleabihf-cross" CACHE PATH "Toolchain root folder." )

set(CMAKE_SYSROOT "${TOOLCHAIN_ROOT}/armv7l-linux-musleabihf" CACHE PATH "Cross-compile system root folder.")
set(CMAKE_C_COMPILER "${TOOLCHAIN_ROOT}/bin/armv7l-linux-musleabihf-gcc${exe}" CACHE FILEPATH "C cross-compiler")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/bin/armv7l-linux-musleabihf-g++${exe}" CACHE FILEPATH "C++ cross-compiler")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
