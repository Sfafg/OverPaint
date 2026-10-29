cmake_minimum_required(VERSION 3.30.1)
project(OverPaint)
set(CMAKE_CXX_STANDARD 23)

option(OVER_PAINT_BUILD_TESTS "Build tests" ON)

set(TESTS_ROOT "${PROJECT_SOURCE_DIR}/tests")
set(SRC_ROOT "${PROJECT_SOURCE_DIR}/src")
set(RESOURCES_ROOT "${PROJECT_SOURCE_DIR}/resources")
set(RESOURCES_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")
set(SHADER_ROOT "${SRC_ROOT}/shaders")
set(SHADER_OUTPUT_DIR "${RESOURCES_OUTPUT_DIR}/shaders")

file(MAKE_DIRECTORY ${TESTS_ROOT} ${SRC_ROOT} ${RESOURCES_ROOT} ${RESOURCES_OUTPUT_DIR} ${SHADER_ROOT} ${SHADER_OUTPUT_DIR})

# Library
include(FetchContent)
FetchContent_Declare(
  VGraphics
  GIT_REPOSITORY "https://github.com/Sfafg/VGraphics.git"
  GIT_TAG main
)
FetchContent_MakeAvailable(VGraphics)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
  CMAKE_CACHE_ARGS
    -DGLFW_BUILD_WAYLAND:BOOL=ON
    -DGLFW_BUILD_X11:BOOL=OFF
    -DGLFW_BUILD_DOCS:BOOL=OFF
)
FetchContent_MakeAvailable(glfw)


file(GLOB_RECURSE SRC "${SRC_ROOT}/*.cpp")
file(GLOB_RECURSE HEADER "${SRC_ROOT}/*.h")

add_library(OverPaintLib STATIC)

target_link_libraries(OverPaintLib PUBLIC VGraphics glm glfw)
target_sources(OverPaintLib
    PRIVATE
        ${SRC}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS src
        FILES ${HEADER}
)
target_precompile_headers(OverPaintLib PUBLIC "${VGraphics_SOURCE_DIR}/src/VG/VG.h")
target_include_directories(OverPaintLib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

if(OVER_PAINT_BUILD_TESTS)
# SHADERS
    find_program(GLSLC_EXECUTABLE glslc)
    if(NOT GLSLC_EXECUTABLE)
        message(FATAL_ERROR "glslc not found. Set GLSLC_EXECUTABLE manually or add to PATH.")
    endif()

    file(GLOB_RECURSE SHADERS "${SHADER_ROOT}/*.vert" "${SHADER_ROOT}/*.frag" "${SHADER_ROOT}/*.comp")
    file(GLOB_RECURSE SHADER_DEPEND "${SHADER_ROOT}/*.glsl")

    foreach(SHADER ${SHADERS})
        string(REPLACE "${SHADER_ROOT}/" "" SHADER_NAME "${SHADER}")
        add_custom_command(
            OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv"
            COMMAND "${GLSLC_EXECUTABLE}" -g -O0 -Werror "${SHADER}" -o "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv"
            DEPENDS "${SHADER}" "${SHADER_DEPEND}"
            COMMENT "Compiling Shader: ${SHADER_NAME}.spv"
        )
        list(APPEND SHADER_DEPEND "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv")
    endforeach()

    add_custom_target(Shaders ALL DEPENDS "${SHADER_DEPEND}")

# RESOURCES
    file(GLOB_RECURSE RESOURCES "${RESOURCES_ROOT}/*.*")

    foreach(RESOURCE ${RESOURCES})
        string(REPLACE "${RESOURCES_ROOT}/" "" RESOURCE_NAME "${RESOURCE}")
        add_custom_command(
            OUTPUT "${RESOURCES_OUTPUT_DIR}/${RESOURCE_NAME}"
            COMMAND ${CMAKE_COMMAND} -E copy "${RESOURCE}" "${RESOURCES_OUTPUT_DIR}/${RESOURCE_NAME}"
            DEPENDS "${RESOURCE}"
            COMMENT "Copying resource: ${RESOURCE_NAME}"
        )
        list(APPEND RESOURCE_DEPEND "${RESOURCES_OUTPUT_DIR}/${RESOURCE_NAME}")
    endforeach()

    add_custom_target(Resources ALL DEPENDS ${RESOURCE_DEPEND})

    # OverPaint
    add_executable(OverPaint ${TESTS_ROOT}/Source.cpp)
    target_link_libraries(OverPaint PRIVATE OverPaintLib glfw glm)
    add_dependencies(OverPaint Shaders Resources)
endif()
