cmake_minimum_required(VERSION 2.8)

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Libraries/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Binaries/Jump&Run/)

add_definitions(-DLINUX)
add_definitions(-std=c++0x) # for nullptr

add_subdirectory(Engine)
add_subdirectory(Framework)
add_subdirectory(Jump&Run)
