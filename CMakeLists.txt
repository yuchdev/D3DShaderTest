cmake_minimum_required(VERSION 3.0)
project(vbos_d3d_shaders)
set(TARGET vbos_d3d_shaders)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

include(${CMAKE_SOURCE_DIR}/cmake/FindDirectX.cmake)

find_package(DirectX)

if(WIN32)
    message("Windows configuraion: enable all exceptions, all warnings")
    set(MY_BOOST_DIR ${WINDOWS_BOOST_DIR})
    set(CMAKE_CXX_FLAGS "/EHa /W1 /MP")    
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

    # define WIN32 macro version
    macro(get_WIN32_WINNT version)
        if (WIN32 AND CMAKE_SYSTEM_VERSION)
            set(ver ${CMAKE_SYSTEM_VERSION})
            string(REPLACE "." "" ver ${ver})
            string(REGEX REPLACE "([0-9])" "0\\1" ver ${ver})

            set(${version} "0x${ver}")
        endif()
    endmacro()

    message("Set WINVER: " ver)
    message("Set SCL_SECURE_NO_WARNINGS")
    get_WIN32_WINNT(ver)
    add_definitions(-D_WIN32_WINNT=${ver})
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)

    # Use folders to sort out projects in VS solution
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
    message(FATAL_ERROR "Windows required for Direct3D support")
endif()

add_subdirectory(triangle)