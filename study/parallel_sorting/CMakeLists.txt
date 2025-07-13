cmake_minimum_required(VERSION 3.10)
project(parallel_sorting)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавьте все исходные файлы, которые нужны для сборки
add_executable(sort
    src/main.cpp
    src/ThreadPool.cpp
)

target_include_directories(sort PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(sort PROPERTIES
    COMPILE_OPTIONS "-g;-O3"
)
