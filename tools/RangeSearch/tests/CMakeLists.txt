
file(GLOB_RECURSE TEST_SOURCES 
    CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)


add_executable(run_tests ${TEST_SOURCES})


target_include_directories(run_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../src       
        ${CMAKE_CURRENT_SOURCE_DIR}/../include   
)

target_link_libraries(run_tests
    PRIVATE
        gtest
)
