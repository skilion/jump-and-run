cmake_minimum_required(VERSION 2.8)

IF (NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE Debug)
ENDIF()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Libraries/${CMAKE_BUILD_TYPE})

add_subdirectory(freetype2)
add_subdirectory(libjpeg)
add_subdirectory(libogg)
add_subdirectory(libpng)
add_subdirectory(libvorbis)
add_subdirectory(zlib)
