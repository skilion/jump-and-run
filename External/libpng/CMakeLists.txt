project(png C)
include_directories(../zlib)
file(GLOB_RECURSE Sources "*.c")
add_library(png STATIC ${Sources})
