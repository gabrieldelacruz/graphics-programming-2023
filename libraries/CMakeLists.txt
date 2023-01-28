# obtain the list of subdirectories
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

FOREACH(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
ENDFOREACH()
