cmake_minimum_required(VERSION 3.24)
project(LearnOpenGL)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GLFW headers
include_directories("C:/glfw-3.4/glfw-3.4/include")

# Include GLAD headers
include_directories("${CMAKE_SOURCE_DIR}/external/glad/include")

# Add GLFW as a subdirectory
add_subdirectory("C:/glfw-3.4/glfw-3.4" "${CMAKE_BINARY_DIR}/glfw")

# Define the executable with GLAD source
add_executable(LearnOpenGL main.cpp
        shaders/shaders.h
        structs/shapes.h
        ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c)

# Link GLFW and any other system libraries required by GLFW
target_link_libraries(LearnOpenGL glfw)
