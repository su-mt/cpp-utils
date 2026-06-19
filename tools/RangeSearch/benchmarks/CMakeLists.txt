
file(GLOB_RECURSE BENCH_SOURCES 
    CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(run_benchmarks ${BENCH_SOURCES})


target_include_directories(run_benchmarks
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)


target_link_libraries(run_benchmarks
    PRIVATE
        benchmark::benchmark
        Boost::boost
)
