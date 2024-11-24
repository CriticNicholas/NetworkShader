add_executable(main main.cpp)

target_link_libraries(main glfw OpenGL::GL ${OPENSSL_LIBRARIES} glad)
