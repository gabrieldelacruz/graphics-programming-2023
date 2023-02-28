
SET(target_inc "")
FILE(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR}/include/ituGL ${CMAKE_CURRENT_LIST_DIR}/include/ituGL/*)
FOREACH(subdir ${subdirectories})
	file(GLOB_RECURSE subdir_inc "${CMAKE_CURRENT_LIST_DIR}/include/ituGL/${subdir}/*.h" )
	source_group(${subdir} FILES ${subdir_inc})
	LIST(APPEND target_inc ${subdir_inc})
ENDFOREACH()

SET(target_src "")
FILE(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR}/src/ituGL ${CMAKE_CURRENT_LIST_DIR}/src/ituGL/*)
FOREACH(subdir ${subdirectories})
	file(GLOB_RECURSE subdir_src "${CMAKE_CURRENT_LIST_DIR}/src/ituGL/${subdir}/*.cpp" )
	source_group(${subdir} FILES ${subdir_src})
	LIST(APPEND target_src ${subdir_src})
ENDFOREACH()

add_library(itugl STATIC ${target_inc} ${target_src})
