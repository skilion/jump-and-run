project(zlib C)
file(GLOB_RECURSE Sources "*.c")
add_library(zlib STATIC ${Sources})
