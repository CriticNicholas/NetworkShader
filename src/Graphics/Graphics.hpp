#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<map>
#include<atomic>
#include<iostream>
#include<functional>

class Window
{
    public:
        Window(unsigned int Width, unsigned int Height, std::string_view WindowName, bool WindowResize = 0)
        {
            glfwInit();

            if (!WindowResize)
            {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            }

            this->ObjectWindow = glfwCreateWindow(Width, Height, WindowName.data(), nullptr, nullptr);

            glfwMakeContextCurrent(this->ObjectWindow);

            glfwSetWindowUserPointer(this->ObjectWindow, this);

            if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) || !this->ObjectWindow)
            {
                glfwDestroyWindow(this->ObjectWindow);
                glfwTerminate();
            }

            glfwSetKeyCallback(this->ObjectWindow, CKeyCallBack);
        }
        ~Window()
        {
            glDeleteBuffers(1, &this->VBO);
            glfwDestroyWindow(this->ObjectWindow);
            glfwTerminate();
        }
        void UseVertives(float vertices[])
        { 
            glGenBuffers(1, &this->VBO);
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
            
            glEnableVertexAttribArray(0);
        }

        template<typename Function, typename... Argc>
        void Start(Function function, Argc&&... argc)
        {
            function(std::forward<Argc>(argc)...);
        }

        template<typename _Function>
        void KeyCallback(int Key, _Function Func)
        {
            Map[Key] = Func;

            glfwSetKeyCallback(ObjectWindow, CKeyCallBack);
        }

        GLFWwindow* ObjectWindow;

    private:
        static void CKeyCallBack(GLFWwindow* GlfwWindow, int Key, int Scancode, int Action, int Mods)
        {
            Window* CurrentWindow = static_cast<Window*>(glfwGetWindowUserPointer(GlfwWindow));

            if (CurrentWindow->Map[Key] && Action == GLFW_PRESS)
            {
                CurrentWindow->Map[Key]();
            }
        }

        GLuint VBO;
        
        std::map<int, std::function<void()>> Map;
};