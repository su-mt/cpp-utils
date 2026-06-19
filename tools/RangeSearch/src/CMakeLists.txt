set(APP_NAME ${CMAKE_PROJECT_NAME})

# Getting all files and directory in user directory
file(GLOB_RECURSE USER_DIR_ITEMS LIST_DIRECTORIES TRUE CONFIGURE_DEPENDS "./*")

# Getting .c file and subdirectory
foreach(USER_DIR_ITEM ${USER_DIR_ITEMS})
    if (IS_DIRECTORY ${USER_DIR_ITEM})
        list(APPEND USER_SUBDIRECTORY ${USER_DIR_ITEM})
    elseif("${USER_DIR_ITEM}" MATCHES ".*(.c|.cpp|.s|.S|.asm|.hpp)$")
        list(APPEND USER_SOURCES ${USER_DIR_ITEM})
    endif()
endforeach()
list(APPEND USER_SOURCES "../include/gui.hpp") # Add the specific header needed by MOC

add_executable(${APP_NAME}
    ${USER_SOURCES}
)

target_include_directories(${APP_NAME}
    PRIVATE
        .
        ${USER_SUBDIRECTORY}
        ${CMAKE_CURRENT_SOURCE_DIR}           
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)



target_link_libraries(${APP_NAME}
    PRIVATE
        Boost::boost
        Qt5::Core
        Qt5::Widgets
)

target_link_directories(${APP_NAME}
    PRIVATE
        .
)
