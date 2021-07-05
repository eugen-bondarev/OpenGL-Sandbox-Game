cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD						17)
set(PROJ_PROJECT_VERSION					0.3.0)
set(PROJ_PROJECT_NAME						Naturaforge)
set(PROJ_APP_NAME							Naturaforge)
set(PROJ_SRC_DIR_NAME						src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 			${CMAKE_BINARY_DIR}/bin)

set(GLEW_INCLUDE_DIR						external/glew/include)
set(GLM_INCLUDE_DIR							external/glm)

project(${PROJ_PROJECT_NAME} VERSION ${PROJ_PROJECT_VERSION})

add_compile_definitions(
	# NF_ENABLE_PROFILING
	# NF_ENABLE_GPU_SYNC
	NF_SHOW_DEBUG_GUI
	WERWEL_USE_GLM
	LINOW_USE_GLM
)

file(GLOB_RECURSE src ${PROJ_SRC_DIR_NAME}/*.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	file(COPY Assets DESTINATION 			${CMAKE_BINARY_DIR}/bin)
	set(CMAKE_CXX_STANDARD_LIBRARIES 		"-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
	set(CMAKE_EXE_LINKER_FLAGS 				"${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
	file(COPY Assets DESTINATION ${CMAKE_BINARY_DIR}/bin)
endif()

add_executable(${PROJ_APP_NAME} ${src})

target_precompile_headers(${PROJ_APP_NAME} PUBLIC ${PROJ_SRC_DIR_NAME}/Pch/Pch.h)

add_subdirectory(external/glfw)
add_subdirectory(external/boxer)
add_subdirectory(external/werwel)
add_subdirectory(external/glew/build/cmake)

target_include_directories(${PROJ_APP_NAME} PUBLIC 
	external/glfw/include 
	external/boxer/include 
	external/werwel/Include 
	${GLM_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR} 
	${CMAKE_SOURCE_DIR}/src
	${PROJ_SRC_DIR_NAME}/external
)

target_link_libraries(werwel glew_s ole32 oleaut32 imm32)
target_link_libraries(${PROJ_APP_NAME} glfw glew_s Boxer werwel)

target_include_directories(werwel PUBLIC ${GLEW_INCLUDE_DIR})
target_include_directories(werwel PUBLIC ${GLM_INCLUDE_DIR})