cmake_minimum_required(VERSION 2.8)

project(benchmark)

include_directories(${CMAKE_SOURCE_DIR})

find_package(Git)
if (GIT_FOUND)
  message("git found: ${GIT_EXECUTABLE}")
else()
    message(FATAL git was not found)
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/frozen-src)
execute_process(
    COMMAND ${GIT_EXECUTABLE} clone https://github.com/serge-sans-paille/frozen.git
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/frozen-src
    ERROR_QUIET
)
include_directories("${CMAKE_BINARY_DIR}/frozen-src/frozen/include")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/hayai-src)
execute_process(
    COMMAND ${GIT_EXECUTABLE} clone https://github.com/nickbruun/hayai.git
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/hayai-src
    ERROR_QUIET
)
include_directories("${CMAKE_BINARY_DIR}/hayai-src/hayai/src")

if (NOT DEFINED BENCHMARK_N)
    set(BENCHMARK_N 100)
endif()

foreach(i RANGE ${BENCHMARK_N})
    set(frozen_map "${frozen_map} {\"${i}\"_s,${i}},\n")
    set(strswitch_map "${strswitch_map} (\"${i}\", return ${i})\n")
    set(benchmark_strings "${benchmark_strings} \"${i}\", ")
endforeach(i)

#message("benchmark_n: ${BENCHMARK_N}")
#message("frozen_map: ${frozen_map}")
#message("strswitch_map: ${strswitch_map}")
#message("benchmark_strings: ${benchmark_strings}")

configure_file(frozen_benchmark.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/frozen_benchmark.cpp)
configure_file(strswitch_benchmark.cpp.in ${CMAKE_CURRENT_SOURCE_DIR}/strswitch_benchmark.cpp)

add_executable(strswitch_benchmark strswitch_benchmark.cpp)
#add_dependencies(strswitch_benchmark hayai)
add_test(strswitch_benchmark strswitch_benchmark)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconstexpr-depth=65536 -std=c++14")

add_executable(frozen_benchmark frozen_benchmark.cpp)
#add_dependencies(frozen_benchmark hayai frozen)
add_test(frozen_benchmark frozen_benchmark)
