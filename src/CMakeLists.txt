set(CMAKE_CXX_FLAGS "-Wall -std=c++1y -pthread -DDEBUG_FD_LIVETIME -DDEBUG_TIMER -DDEBUG_PROXY_MAP")
#-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(PkgConfig REQUIRED)

include_directories(
	${CMAKE_SOURCE_DIR}/include/
)

include_directories(

)

add_executable(qcdump
	main.cc
)



