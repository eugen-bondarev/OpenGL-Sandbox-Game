cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 				17)

set(PROJ_PROJECT_VERSION   		0.1.7)
set(PROJ_PROJECT_NAME      		Naturaforge)
set(PROJ_APP_NAME          		Naturaforge)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 	${CMAKE_BINARY_DIR}/bin)
set(GLEW_INCLUDE_DIR 								External/glew/include)
set(PROJ_SRC_DIR_NAME      					Source)

project(${PROJ_PROJECT_NAME} VERSION ${PROJ_PROJECT_VERSION})

file(GLOB_RECURSE src ${PROJ_SRC_DIR_NAME}/*.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
endif()

add_executable(${PROJ_APP_NAME} ${src})

target_precompile_headers(${PROJ_APP_NAME} PUBLIC ${PROJ_SRC_DIR_NAME}/Pch/Pch.h)

add_subdirectory(External/glfw)
add_subdirectory(External/Werwel)
add_subdirectory(External/glew/build/cmake)

target_include_directories(${PROJ_APP_NAME} PUBLIC 
	External/glfw/include 
	External/Werwel/Include 
	External/glm
	${GLEW_INCLUDE_DIR} 
	${CMAKE_SOURCE_DIR}/Source
	${PROJ_SRC_DIR_NAME}/External
)
target_link_libraries(${PROJ_APP_NAME} glfw glew_s Werwel)

target_include_directories(Werwel PUBLIC ${GLEW_INCLUDE_DIR})
target_link_libraries(Werwel glew_s ole32 oleaut32 imm32)