## i have cmake 3.5
cmake_minimum_required(VERSION 3.5)
project(client_server)

## use this to globally use C++11 with in our project
set(CMAKE_CXX_STANDARD 11)

## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)

## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

# set(SOURCE_FILES basic_example.cpp)
add_executable(zmq_server "zmq_server.cpp")

## add the include directory to our compile directives
## at the 0mq library to our link directive
target_include_directories(zmq_server PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(zmq_server PUBLIC ${ZeroMQ_LIBRARY})

configure_file("${CMAKE_SOURCE_DIR}/zmq_client.py" "${CMAKE_CURRENT_BINARY_DIR}/zmq_client.py")
configure_file("${CMAKE_SOURCE_DIR}/run.sh" "${CMAKE_CURRENT_BINARY_DIR}/run.sh")