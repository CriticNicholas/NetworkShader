# NetworkShader
NetworkShader is a small C++ project designed to show shaders transmitted both over the network and through files. 

Several libraries are used:
1. [glfw](https://github.com/glow/glow ) - C library for work with OpenGL, Vulkan 
2. [asio](https://github.com/chriskohlhoff/asio ) - C++ library for Socket
3. [Glad](https://github.com/Dav1dde/glad/tree/glad2) - Multilanguage library for load OpenGL functions

For Use:
* Install glfw to environment in your O.S
* Compiling:
```
git clone https://github.com/CriticNicholas/NetworkShader.git
cd NetworkShader && cmake -B build -G Ninja
ninja -C build
```
* Start:
```
cd build\build\src\
#run the executable main file
```