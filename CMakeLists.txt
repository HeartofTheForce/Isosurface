cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

set(OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})

find_package(OpenGL REQUIRED)

# glad
add_subdirectory(external/glad)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/GLFW)

#glm
add_subdirectory(external/glm)

include_directories(SYSTEM 
    external/stb/include
    external/PerlinNoise/include
)
include_directories(include)

add_compile_definitions(GLM_FORCE_LEFT_HANDED GLM_FORCE_XYZW_ONLY)

file(GLOB_RECURSE sources
    src/*.cpp
    src/*.c
)

add_executable(${PROJECT_NAME} ${sources})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-error -Wall -Wextra -Weffc++ -fopenmp -O2)
target_link_libraries(${PROJECT_NAME} glad glfw glm gomp OpenGL::GL)

add_custom_target(assets COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${OUTPUT_PATH}/assets BYPRODUCTS ${OUTPUT_PATH}/assets)
add_dependencies(${PROJECT_NAME} assets)

