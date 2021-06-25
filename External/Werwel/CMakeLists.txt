cmake_minimum_required(VERSION 3.16)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -std=c++17")
	set(CMAKE_CXX_COMPILER_FORCED TRUE)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} -std=c++17")
	set(CMAKE_CXX_COMPILER_FORCED TRUE)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "/std:c++17 /EHsc")
endif()

project(Werwel VERSION 0.0.1)

file(GLOB src 
  Source/*.cpp
  Source/**/*.cpp
  Source/**/**/*.cpp
)

add_library(Werwel STATIC ${src})

target_include_directories(Werwel PUBLIC include/Werwel)
target_include_directories(Werwel PUBLIC ${GLEW_INCLUDE_DIR})