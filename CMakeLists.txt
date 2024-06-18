cmake_minimum_required(VERSION 3.28)
project(http_test)

set(CMAKE_CXX_STANDARD 17)
link_libraries(ws2_32)

add_executable(http_test_server win/main1.cpp)
add_executable(http_test_client win/main2.cpp)
