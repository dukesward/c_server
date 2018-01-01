message( STATUS "linking library: socket_c")
add_library(socket_c
	connect.h
	connect.c
	nonblock.h
	nonblock.c
)