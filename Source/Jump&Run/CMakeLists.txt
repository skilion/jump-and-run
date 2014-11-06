project(JumpAndRun CXX)

include_directories(../Engine)
include_directories(../Framework)
link_directories(${CMAKE_HOME_DIRECTORY}/../Libraries/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE Sources "*.cpp")
add_executable(JumpAndRun ${Sources})

target_link_libraries(JumpAndRun engine framework)
target_link_libraries(JumpAndRun freetype2 jpeg png vorbis ogg zlib)
target_link_libraries(JumpAndRun GL GLU openal X11 Xxf86vm)
