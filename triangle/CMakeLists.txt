set(TARGET CXX triangle)

if(DirectX_D3D8_INCLUDE_FOUND)
    message("Add Direct3D 8 includes: " ${DirectX_D3D8_INCLUDE_DIR})
    include_directories(${DirectX_D3D8_INCLUDE_DIR})
endif()

if(DirectX_D3D9_INCLUDE_FOUND)
    message("Add Direct3D 9 includes: " ${DirectX_D3D9_INCLUDE_DIR})
    include_directories(${DirectX_D3D9_INCLUDE_DIR})
endif()


add_executable(${TARGET} triangle.cpp)
target_link_libraries(${TARGET} d3d9)
