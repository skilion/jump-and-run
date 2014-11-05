cmake_minimum_required(VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Libraries/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Binaries/${CMAKE_BUILD_TYPE})

add_definitions(-DLINUX)

add_subdirectory(Engine)
add_subdirectory(Framework)
add_subdirectory(Jump&Run)
