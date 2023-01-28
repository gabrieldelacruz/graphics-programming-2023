
file(GLOB_RECURSE target_inc "include/*.h" )
file(GLOB_RECURSE target_src "src/*.cpp" )
add_library(itugl STATIC ${target_inc} ${target_src})