cmake_minimum_required(VERSION 3.10)

project(NetworkShader)

if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 17)
endif ()

find_package(glfw3 3.4 REQUIRED)

find_package(OpenGL REQUIRED)

find_package(OpenSSL REQUIRED)

include_directories(include/ include/asio/asio/include ${OPENSSL_INCLUDE_DIR})

add_library(glad include/glad/glad.h include/glad.c)

add_subdirectory(src)