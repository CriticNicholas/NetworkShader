#pragma once

#include<glad/glad.h>
#include<iostream>

class Shader
{

    public:
        Shader(std::string& Vertex, std::string& Fragment)
        {
            CompileShader(GL_VERTEX_SHADER, Vertex);
            unsigned VertexShader = ShaderId;
            
            CompileShader(GL_FRAGMENT_SHADER, Fragment);
            unsigned FragmentShader = ShaderId;

            this->ProgramId = glCreateProgram();

            glAttachShader(this->ProgramId, VertexShader);
            glAttachShader(this->ProgramId, FragmentShader);
            glLinkProgram(this->ProgramId);
            glValidateProgram(this->ProgramId);

            int Status;

            glGetProgramiv(this->ProgramId, GL_LINK_STATUS, &Status);

            if (!Status)
            {
                char CharStatus[512];
                glGetProgramInfoLog(this->ProgramId, 512, nullptr, CharStatus);

                std::cout << "Error: " << Status << std::endl << CharStatus << std::endl;
            }

            glDeleteShader(VertexShader);
            glDeleteShader(FragmentShader);
        }

        void Use()
        {
            glUseProgram(this->ProgramId);
        }

        ~Shader() 
        {
            glDeleteProgram(this->ProgramId);
        }

        unsigned ProgramId;

    private:
        void CompileShader(unsigned int ShaderType, std::string& Src)
        {
            this->ShaderId = glCreateShader(ShaderType);
            const char* CString = Src.c_str();

            glShaderSource(this->ShaderId, 1, &CString, nullptr);
            glCompileShader(this->ShaderId);

            int Status;

            glGetShaderiv(this->ShaderId, GL_COMPILE_STATUS, &Status);

            if (!Status)
            {
                char CharStatus[512];
                glGetShaderInfoLog(this->ShaderId, 512, nullptr, CharStatus);
                    
                std::cout << ShaderType << std::endl << CharStatus << std::endl;
            }
        } 

        unsigned ShaderId;
};