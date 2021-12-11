cmake_minimum_required(VERSION 3.1)

project(IK)

set(CMAKE_CXX_STANDARD 11)

configure_file(CMakeProjectMacros.h.in Project-macros/CMakeProjectMacros.h)

add_executable(IK 
            Source/main.cpp 
            Source/glad.c 
            OpenGLTypes/Shader.cpp 
            OpenGLTypes/Shader.h
            OpenglTypes/Texture.h
            OpenglTypes/TextureParameters.h
            OpenglTypes/Texture.cpp 
            OpenglTypes/TImageLoader.cpp
            Dependencies/stb_image/stb_image.cpp
         )

include_directories(Dependencies Dependencies/Include  "${PROJECT_SOURCE_DIR}/Source" "${PROJECT_BINARY_DIR}/Project-macros" OpenGLTypes)

find_library(GLFW_LIBRARY glfw3 glfw3.lib "${PROJECT_SOURCE_DIR}/Dependencies/Lib")

target_link_libraries(IK LINK_PUBLIC ${GLFW_LIBRARY} opengl32.lib)



