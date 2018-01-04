message( STATUS "linking library: socket_c")
add_library(socket_c
	include.h
	connect.h
	connect.c
	utils.h
	utils.c
	config.h
	config.c
)