set(CMAKE_CXX_FLAGS "-Wall -std=c++1y -pthread")
#-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(LibEv REQUIRED)
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(SSL openssl)

include_directories(
	${CMAKE_SOURCE_DIR}/include/
)

add_executable(ss-ev++
	main.cc
	server.cc
)

target_link_libraries(ss-ev++
	${LIBEV_LIBRARY}
)


