cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
add_compile_options(-fcolor-diagnostics -Wall -fansi-escape-codes -g -Wno-deprecated)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/glm
    ${CMAKE_SOURCE_DIR}/includes/assimp
    ${CMAKE_SOURCE_DIR}/src/Utility
)

# Find required packages
find_package(OpenGL REQUIRED)

# Add source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/WindowSystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/EventSystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/CameraSystem/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Utility/Clock/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/Utility/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/buffers/VertexArrayBuffer.cpp"
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/buffers/Vertexbuffer.cpp"
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/Textures/TextureSystem.cpp"
    "${CMAKE_SOURCE_DIR}/glad.c"
)

# Create executable
add_executable(app ${SOURCES})

# Link libraries
target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/libs/libassimp.5.4.1.dylib
    ${CMAKE_SOURCE_DIR}/libglfw3.a
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)

# Set output directory
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)