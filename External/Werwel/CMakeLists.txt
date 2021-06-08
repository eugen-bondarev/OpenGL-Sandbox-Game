cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc")

project(Werwel VERSION 0.0.1)

file(GLOB src 
  Source/*.cpp
  Source/**/*.cpp
  Source/**/**/*.cpp
)

add_library(Werwel STATIC ${src})

target_include_directories(Werwel PUBLIC include/Werwel)
target_include_directories(Werwel PUBLIC ${GLEW_INCLUDE_DIR})