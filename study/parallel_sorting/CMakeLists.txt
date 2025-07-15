cmake_minimum_required(VERSION 3.10)
project(parallel_sorting)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sort
    src/main.cpp
    src/ThreadPool.cpp
)

target_include_directories(sort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Оптимизации и отладочная информация
target_compile_options(sort PRIVATE -O3 -g -Wall -Wextra)
