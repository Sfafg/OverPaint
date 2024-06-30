cmake_minimum_required (VERSION 3.23.3)
project (OverPaint)
set(CMAKE_CXX_STANDARD 23)
add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)
 
set(TESTS_ROOT ${PROJECT_SOURCE_DIR}/tests)
set(SRC_ROOT ${PROJECT_SOURCE_DIR}/src)
set(SHADER_ROOT ${SRC_ROOT}/shaders)

set(SRC
    ${SRC_ROOT}/Window.cpp
    ${SRC_ROOT}/Spline.cpp
    ${SRC_ROOT}/Input.cpp
)

# Main executable
add_executable (OverPaint ${TESTS_ROOT}/Source.cpp ${SRC})
add_custom_command(TARGET OverPaint PRE_BUILD
    COMMAND  ../CompileShaders.bat
)
target_link_libraries(OverPaint PUBLIC VGraphics glfw3)
target_include_directories(OverPaint PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)
target_link_directories(OverPaint PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/"
)

# SpanSet Test
add_executable (SpanTest ${TESTS_ROOT}/SpanSetTest.cpp)
target_include_directories(SpanTest PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)

# Spline Test
add_executable (SplineTest ${TESTS_ROOT}/SplineTest.cpp ${SRC})
add_custom_command(TARGET SplineTest PRE_BUILD
    COMMAND  ../CompileShaders.bat
)
target_link_libraries(SplineTest PUBLIC VGraphics glfw3)
target_include_directories(SplineTest PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
)
target_link_directories(SplineTest PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/"
)