project(jpeg C)
file(GLOB_RECURSE Sources "*.c")
add_library(jpeg STATIC ${Sources})
