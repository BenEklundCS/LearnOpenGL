cmake_minimum_required(VERSION 3.10)
project(OpenGL_Project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(OpenGL_Project main.cpp)

# Include GLFW and GLAD
# Adjust these paths if your structure is different or you are using system packages
add_subdirectory(/home/ben/Programming/OpenGL/glfw ${CMAKE_BINARY_DIR}/glfw)  # Path to GLFW source and binary directory
add_library(GLAD external/glad/src/glad.c)  # Assuming GLAD is under external/glad

# Include directories
target_include_directories(OpenGL_Project PRIVATE
        external/glad/include
        /home/ben/Programming/OpenGL/glfw/include
        shaders
        structs
)

# Link libraries
target_link_libraries(OpenGL_Project
        GLAD
        glfw
        ${CMAKE_DL_LIBS}    # Link dynamic libraries if needed
)

# If on Windows, link OpenGL32
if (WIN32)
    target_link_libraries(OpenGL_Project opengl32)
endif()

# If on macOS, link OpenGL and Cocoa/IOKit for GLFW
if (APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(CORE_FOUNDATION_LIBRARY CoreFoundation REQUIRED)
    find_library(CORE_VIDEO_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(OpenGL_Project
            ${COCOA_LIBRARY}
            ${IOKIT_LIBRARY}
            ${CORE_FOUNDATION_LIBRARY}
            ${CORE_VIDEO_LIBRARY}
    )
endif()