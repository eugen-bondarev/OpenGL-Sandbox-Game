cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

project(werwel VERSION 0.0.1)

file(GLOB src 
  src/*.cpp
  src/**/*.cpp
  src/**/**/*.cpp
)

add_library(werwel STATIC ${src})

target_precompile_headers(werwel PUBLIC include/werwel/pch.h)

target_include_directories(werwel PUBLIC include/werwel)
target_include_directories(werwel PUBLIC ${GLEW_INCLUDE_DIR})
target_include_directories(werwel PUBLIC ${GLM_INCLUDE_DIR})