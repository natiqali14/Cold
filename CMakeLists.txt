cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags
if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM x64)
    add_definitions(-DWIN=1)
    add_compile_options(/W1)
endif(WIN32)
if(APPLE)
    add_compile_options(-fcolor-diagnostics -Wall -fansi-escape-codes -g -Wno-deprecated)   
endif(APPLE)



# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/includes
    ${CMAKE_SOURCE_DIR}/includes/glm
    ${CMAKE_SOURCE_DIR}/includes/assimp
    ${CMAKE_SOURCE_DIR}/src/Utility
    ${CMAKE_SOURCE_DIR}/src/Utility/Clock
    ${CMAKE_SOURCE_DIR}/src/third_party
    ${CMAKE_SOURCE_DIR}/src/opengl_backend
    ${CMAKE_SOURCE_DIR}/src/opengl_backend/Textures
    ${CMAKE_SOURCE_DIR}/src/opengl_backend/buffers
    ${CMAKE_SOURCE_DIR}/src/opengl_backend/Geometry
    ${CMAKE_SOURCE_DIR}/src/opengl_backend/Utils
    ${CMAKE_SOURCE_DIR}/src/CameraSystem
    ${CMAKE_SOURCE_DIR}/src/EventSystem
    ${CMAKE_SOURCE_DIR}/src/opengl_backend/Shaders
    ${CMAKE_SOURCE_DIR}/src/ShaderCode
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/third_party/ImGUI
    ${CMAKE_SOURCE_DIR}/src/WindowSystem
    ${CMAKE_SOURCE_DIR}/src/EventSystem
    

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
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/Shaders/ShaderSystem.cpp"
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/Geometry/*/cpp"
    "${CMAKE_SOURCE_DIR}/src/opengl_backend/Utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/glad.c"
    "${CMAKE_SOURCE_DIR}/src/third_party/ImGUI/*.cpp"
)

set(CMAKE_BUILD_TYPE Debug)

# Create executable
add_executable(app ${SOURCES})

# Link libraries
if(APPLE)
    target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/libs/libassimp.5.4.1.dylib
    ${CMAKE_SOURCE_DIR}/libglfw3.a
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)

endif(APPLE)
if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM x64)
    target_link_libraries(app
    ${CMAKE_SOURCE_DIR}/libs/Windows/assimp-vc143-mtd.lib

    ${CMAKE_SOURCE_DIR}/libs/Windows/glfw3.lib
    opengl32
    )
endif(WIN32)


# Set output directory
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Ensure consistency across build types
set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}"
)


# Handle runtime library conflicts if any
if(WIN32)
    target_link_options(app PRIVATE /NODEFAULTLIB:MSVCRT)
endif(WIN32)