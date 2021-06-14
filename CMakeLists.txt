cmake_minimum_required(VERSION 3.16)

set(PROJECTOR_PROJECT_NAME      Forgio)
set(PROJECTOR_PROJECT_VERSION   0.1.2)
set(PROJECTOR_APP_NAME          Forgio)
set(PROJECTOR_SRC_DIR_NAME      Source)
set(CMAKE_CXX_FLAGS             "/std:c++17 /EHsc")

project(${PROJECTOR_PROJECT_NAME} VERSION ${PROJECTOR_PROJECT_VERSION})

file(GLOB src 
	${PROJECTOR_SRC_DIR_NAME}/*.cpp
	${PROJECTOR_SRC_DIR_NAME}/**/*.cpp
	${PROJECTOR_SRC_DIR_NAME}/**/**/*.cpp
	${PROJECTOR_SRC_DIR_NAME}/**/**/**/*.cpp
	${PROJECTOR_SRC_DIR_NAME}/**/**/**/**/*.cpp
	${PROJECTOR_SRC_DIR_NAME}/**/**/**/**/**/*.cpp
)

file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(${PROJECTOR_APP_NAME} ${src})

target_include_directories(${PROJECTOR_APP_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Source)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC ${PROJECTOR_SRC_DIR_NAME}/External)
target_precompile_headers(${PROJECTOR_APP_NAME} PUBLIC ${PROJECTOR_SRC_DIR_NAME}/Pch.h)

add_subdirectory(External/glfw)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/glfw/include)
target_link_libraries(${PROJECTOR_APP_NAME} glfw)

set(GLEW_INCLUDE_DIR External/glew/include)
add_subdirectory(External/glew/build/cmake)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC ${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECTOR_APP_NAME} glew_s)

add_subdirectory(External/Werwel)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/Werwel/Include)
target_include_directories(Werwel PUBLIC ${GLEW_INCLUDE_DIR})
target_link_libraries(${PROJECTOR_APP_NAME} Werwel)

add_subdirectory(External/glm)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/glm)