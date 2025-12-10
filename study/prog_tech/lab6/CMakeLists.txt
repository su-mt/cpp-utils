cmake_minimum_required(VERSION 3.10)
project(StudentManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаемые директории
include_directories(${CMAKE_SOURCE_DIR}/src/include)

# Исходные файлы для программы управления студентами
set(STUDENT_SOURCES
    src/student.cpp
    src/main.cpp
)

# Исходные файлы для программы поиска в файлах
set(FILE_SEARCH_SOURCES
    src/file_stream_search.cpp
)

# Создание исполняемого файла для управления студентами
add_executable(student_manager ${STUDENT_SOURCES})

# Создание исполняемого файла для поиска в файлах
add_executable(file_searcher ${FILE_SEARCH_SOURCES})

# Флаги компилятора для лучшей диагностики
if(MSVC)
    target_compile_options(student_manager PRIVATE /W4)
    target_compile_options(file_searcher PRIVATE /W4)
else()
    target_compile_options(student_manager PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(file_searcher PRIVATE -Wall -Wextra -Wpedantic)
endif()
