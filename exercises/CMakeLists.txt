
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_LIST_DIR})

FOREACH(subdir ${SUBDIRS})
	set(TARGETNAME ${subdir})
    add_subdirectory(${subdir})
	if (TARGET ${TARGETNAME})
		set_target_properties(${TARGETNAME} PROPERTIES
			FOLDER exercises/${subdir}
			VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${subdir})
	endif()
ENDFOREACH()
