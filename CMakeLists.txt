cmake_minimum_required(VERSION 3.5)

project(UdpSocketHelperCpp LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.40.0 REQUIRED system thread regex)

include_directories(
  include
   ${Boost_INCLUDE_DIRS}
)
add_executable(string_udp_sender string_udp_sender.cpp)
target_link_libraries( string_udp_sender
 ${Boost_LIBRARIES}
)


add_executable(string_udp_receiver string_udp_receiver.cpp)
target_link_libraries( string_udp_receiver
 ${Boost_LIBRARIES}
)

add_executable(binary_udp_sender binary_udp_sender.cpp)
target_link_libraries( binary_udp_sender
 ${Boost_LIBRARIES}
)


add_executable(binary_udp_receiver binary_udp_receiver.cpp)
target_link_libraries( binary_udp_receiver
 ${Boost_LIBRARIES}
)

INSTALL (
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")


