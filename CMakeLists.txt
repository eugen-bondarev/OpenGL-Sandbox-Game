cmake_minimum_required(VERSION 3.16)

set(PROJECTOR_PROJECT_NAME      Forgio)
set(PROJECTOR_PROJECT_VERSION   0.0.8)
set(PROJECTOR_APP_NAME          Forgio)
set(PROJECTOR_SRC_DIR_NAME      Source)
set(CMAKE_CXX_FLAGS     "/std:c++17 /EHsc")

project(${PROJECTOR_PROJECT_NAME} VERSION ${PROJECTOR_PROJECT_VERSION})

file(GLOB src 
    ${PROJECTOR_SRC_DIR_NAME}/*.cpp
    ${PROJECTOR_SRC_DIR_NAME}/**/*.cpp
    ${PROJECTOR_SRC_DIR_NAME}/**/**/*.cpp
    ${PROJECTOR_SRC_DIR_NAME}/**/**/**/*.cpp
    ${PROJECTOR_SRC_DIR_NAME}/**/**/**/**/*.cpp
    ${PROJECTOR_SRC_DIR_NAME}/**/**/**/**/**/*.cpp
)

add_executable(${PROJECTOR_APP_NAME} ${src})

target_include_directories(${PROJECTOR_APP_NAME} PUBLIC ${PROJECTOR_SRC_DIR_NAME})
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC ${PROJECTOR_SRC_DIR_NAME}/External)
target_precompile_headers(${PROJECTOR_APP_NAME} PUBLIC ${PROJECTOR_SRC_DIR_NAME}/Pch.h)

add_subdirectory(External/glfw)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/glfw/include)
target_link_libraries(${PROJECTOR_APP_NAME} glfw)

add_subdirectory(External/glew/build/cmake)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/glew/include)
target_link_libraries(${PROJECTOR_APP_NAME} glew_s)

add_subdirectory(External/spdlog)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/spdlog/include)
target_link_libraries(${PROJECTOR_APP_NAME} spdlog)

add_subdirectory(External/glm)
target_include_directories(${PROJECTOR_APP_NAME} PUBLIC External/glm)