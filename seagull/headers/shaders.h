#ifndef SHADERS_HEADER_FILE
#define SHADERS_HEADER_FILE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Utils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
    public:
        unsigned int ID;
        
        Shader(const char *vertexPath, const char *fragmentPath)
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            // reading and storing shader data from files
            try
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e)
            {
                std::cout << "SHADER ERROR: File not successfully read" << std::endl;
            }

            const char *vShaderCode = vertexCode.c_str();
            const char *fShaderCode = fragmentCode.c_str();
            unsigned int vertex, fragment;
            GLint shaderCompileStatus;

            // compile vertex shader
            GLCall(vertex = glCreateShader(GL_VERTEX_SHADER));
            GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
            GLCall(glCompileShader(vertex));
            GLCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &shaderCompileStatus));
            if (!shaderCompileStatus)
            {
                GLchar message[1024];
                GLCall(glGetShaderInfoLog(vertex, 1024, NULL, message));
                std::cout << "VERTEX SHADER ERROR:\nFile: " << vertexPath << "\nProblem: "
                    << message << std::endl;
            }
            // compile fragment shader
            GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
            GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
            GLCall(glCompileShader(fragment));
            GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &shaderCompileStatus));
            if (!shaderCompileStatus)
            {
                GLchar message[1024];
                GLCall(glGetShaderInfoLog(vertex, 1024, NULL, message));
                std::cout << "FRAGMENT SHADER ERROR:\nFile: " << fragmentPath << "\nProblem: "
                    << message << std::endl;
            }

            // Create shader program
            GLCall(ID = glCreateProgram());
            GLCall(glAttachShader(ID, vertex));
            GLCall(glAttachShader(ID, fragment));
            GLCall(glLinkProgram(ID));

            // check for linking errors
            GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &shaderCompileStatus));
            if (!shaderCompileStatus)
            {
                GLchar message[1024];
                GLCall(glGetProgramInfoLog(ID, 1024, NULL, message));
                std::cout << "LINKING SHADER ERROR: " << message << std::endl;
            }

            // Delete shaders as they have been linked to program and are no 
            // longer needed.
            GLCall(glDeleteShader(vertex));
            GLCall(glDeleteShader(fragment));
        };

        void Destroy()
        {
            GLCall(glDeleteProgram(ID));
        }

        void use()
        {
            GLCall(glUseProgram(ID));
        };

        void setBool(const std::string &name, bool value) const
        {
            GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
        }
        void setInt(const std::string &name, int value) const
        {
            GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
        }
        void setFloat(const std::string &name, float value) const
        {
            GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
        }
        // --------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const
        { 
            glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec2(const std::string &name, float x, float y) const
        { 
            glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
        }
        // --------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const
        { 
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
        }
        void setVec3(const std::string &name, float x, float y, float z) const
        { 
            glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
        }
        // --------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const
        {
            GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
        }
        void setVec4(const std::string &name, float x, float y, float z, float w)
        {
            GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w));
        }
        // --------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const
        {
            GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
        }
        // --------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const
        {
            GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
        }
        // --------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const
        {
            GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
        }
};

#endif
