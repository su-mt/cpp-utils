cmake_minimum_required(VERSION 3.22)
project(RangeSearch VERSION 0.1)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_BUILD_TYPE Release)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(
        -fsanitize=address
        -fsanitize=undefined
        -fno-omit-frame-pointer
    )

    add_link_options(
        -fsanitize=address
        -fsanitize=undefined
    )
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTORCC ON) 

# Находим компоненты Qt5
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

find_package(Boost REQUIRED)

include(FetchContent)
FetchContent_Declare( googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare( google_benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4  
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(google_benchmark)


add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)