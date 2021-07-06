cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(mgcwel VERSION 0.0.1)

file(GLOB_RECURSE src src/*.cpp external/imgui/*.cpp)

add_library(mgcwel STATIC ${src})
target_precompile_headers(mgcwel PUBLIC include/mgcwel/pch.h)

add_subdirectory(external/boxer)
add_subdirectory(external/glew/build/cmake)
add_subdirectory(external/glfw)

target_link_libraries(mgcwel glew_s glfw ole32 oleaut32 imm32)

target_include_directories(mgcwel PUBLIC include/mgcwel)
target_include_directories(mgcwel PUBLIC external)
target_include_directories(mgcwel PUBLIC external/glew/include)
target_include_directories(mgcwel PUBLIC external/glfw/include)
target_include_directories(mgcwel PUBLIC external/boxer/include)
target_include_directories(mgcwel PUBLIC external/glm)
