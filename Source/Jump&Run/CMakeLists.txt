project(JumpRun CXX)

#include_directories(.)
include_directories(../Engine)
include_directories(../Framework)
link_directories(${CMAKE_HOME_DIRECTORY}/../Libraries/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE Sources "*.cpp")
add_executable(JumpRun ${Sources})

add_dependencies(JumpRun Engine)
add_dependencies(JumpRun Framework)
target_link_libraries(JumpRun engine framework)
target_link_libraries(JumpRun freetype2 jpeg png vorbis ogg zlib)
target_link_libraries(JumpRun GL GLU openal X11)
