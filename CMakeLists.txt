cmake_minimum_required(VERSION 3.10)
project(DistData)

set(CMAKE_CXX_STANDARD 17)

add_executable(CoordinatorNode src/CoordinatorNode.cpp)
add_executable(DataNode src/DataNode.cpp)
add_executable(Client src/Client.cpp)

# Optionally, add libraries for networking, threading, etc.
# find_package(Threads REQUIRED)
# target_link_libraries(CoordinatorNode PRIVATE Threads::Threads)
# target_link_libraries(DataNode PRIVATE Threads::Threads)
# target_link_libraries(Client PRIVATE Threads::Threads) 