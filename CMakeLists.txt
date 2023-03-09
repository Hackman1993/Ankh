cmake_minimum_required(VERSION 3.22)
project(Ankh)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE CPP_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE CPP_SOURCES source/*.cpp source/*.cc)
file(GLOB_RECURSE CPP_TESTS tests/*.cpp)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)

add_library(ankh ${CPP_HEADERS} ${CPP_SOURCES})
add_executable(AnkhTest ${CPP_TESTS})
target_link_libraries(AnkhTest Boost::unit_test_framework ankh)
