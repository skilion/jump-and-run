project(ogg C)
include_directories(include)
file(GLOB_RECURSE Sources "*.c")
add_library(ogg STATIC ${Sources})
