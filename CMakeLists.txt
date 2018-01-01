####### setup project and cmake #######

# Project name
project (server)

# CMake version requirement
cmake_minimum_required(VERSION 3.0)

# set output target directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#create executable for hello
include_directories(
	${CMAKE_SOURCE_DIR}/socket
)

# add subdirectories
add_subdirectory(socket)

# add executable
set(server_c test.c)
add_executable(server_test ${server_c})

# linke libraries for executable
target_link_libraries(server_test socket_c)