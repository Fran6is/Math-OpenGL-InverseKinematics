
# Rough implementation of inverse kinematics using OpenGL ( for drawing IK chain ) and C++ ( to setup bone hierarchy / movement )

<hr>

***Platform build***: x86 / all configurations

***OpenGL***: GLSL4.3, OpenGL32.lib, glad

***Windowing***: GLFW

***Math library***: GLM

<hr>

## CMake build (With CMake installed and command prompt opened in project's root directory)

- Generate project files for x86 platform ( in this case win32 )
```
[Project root directory]: cmake . -B build -A Win32
```
- Build project
```
[Project root directory]: cmake --build build -t IK
[Project root directory]: cd build/Debug
[Project root directory / build / Debug]: IK
```

<hr>

## Free base
https://user-images.githubusercontent.com/48392004/145690563-22824e41-57a0-4ee9-a53f-714df9b63113.mp4


## Fixed base
https://user-images.githubusercontent.com/48392004/145690620-749b64a4-d9ae-4e2a-87c7-485e3e756fd9.mp4



