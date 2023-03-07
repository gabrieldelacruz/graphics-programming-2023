# ---------------------------------------------------------------------------------
# Assimp lib for .obj files - stripped support to other file formats than OBJ
# ---------------------------------------------------------------------------------


add_compile_definitions(ASSIMP_BUILD_BOOST_WORKAROUND)
add_compile_definitions(OPENDDLPARSER_BUILD)
add_compile_definitions(ASSIMP_BUILD_NO_OWN_ZLIB)
add_compile_definitions(ASSIMP_BUILD_NO_EXPORT)
add_compile_definitions(ASSIMP_BUILD_NO_X_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_AMF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_3DS_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MD3_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MD5_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MDL_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MD2_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_PLY_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_ASE_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_HMP_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_SMD_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MDC_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MD5_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_STL_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_LWO_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_DXF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_NFF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_RAW_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_SIB_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_OFF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_AC_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_BVH_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_IRRMESH_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_IRR_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_Q3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_B3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_COLLADA_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_TERRAGEN_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_CSM_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_LWS_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_OGRE_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_OPENGEX_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MS3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_COB_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_BLEND_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_Q3BSP_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_NDO_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_STEP_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_IFC_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_XGL_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_ASSBIN_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_C4D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_3MF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_X3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_GLTF_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_GLTF2_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_SINGLETHREADED)
add_compile_definitions(ASSIMP_BUILD_NO_M3D_IMPORTER)
add_compile_definitions(ASSIMP_BUILD_NO_MMD_IMPORTER)


IF(WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
    add_compile_definitions(PLATFORM=WINDOWS)
ELSEIF(UNIX AND APPLE)
    add_compile_definitions(PLATFORM_DARWIN)
    add_compile_definitions(PLATFORM=DARWIN)
ELSE()
    add_compile_definitions(PLATFORM_LINUX)
    add_compile_definitions(PLATFORM=LINUX)
ENDIF()


include_directories(
        //${EXTERNAL_LIBRARIES_SOURCE_PATH}/assimp
        assimp
        include
        code
        contrib/zlib
        ./
)


file(GLOB target_src "code/Common/*.cpp"
        "code/PostProcessing/*.cpp"
		"code/Material/*.cpp"
        "code/FBX/*.cpp"
        "code/Obj/*.cpp")


if(FBX_SUPPORT)
    # will build zlib (required by FBX) in the same lib file as assimp
    file(GLOB zlib_target_src "contrib/zlib/*.c")
    add_library(assimp STATIC ${target_src} ${zlib_target_src})
ELSE()
    add_compile_definitions(ASSIMP_BUILD_NO_FBX_IMPORTER)
    add_library(assimp STATIC ${target_src})
ENDIF()


