#ifndef SHADERS_HEADER_FILE
#define SHADERS_HEADER_FILE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Utils.h"
#include "profiler.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


/*
 * Notes:
 *
 * VERTEX SHADER:
 * The vertex shader is what controls the locaions and transformations of a Mesh/Model/whatever is on the screen
 * The shader takes the information that gets loaded in from a model/mesh and
 * sends it straight to the fragment shader or keeps it and set the position of
 * the vertices
 * Think of it as the second layer of the application whereas without it, all
 * that would be there on the screen is a mesh that does nothing. This layer
 * makes it so that it takes that static mesh and does something with it, hiding
 * the static bottom layer.
 *
 * FRAGMENT SHADER:
 * The fragment shader is similar to the vertex shader except that it controls
 * the look and colors of the mesh.You can use this to setup procedural textures
 * or colors.
 */
class Shader 
{
    public:
        unsigned int ID;
        std::string vShaderFile;
        std::string fShaderFile;
        std::string gShaderFile;
        
        Shader() {}
        Shader(std::string vertexPath, std::string fragmentPath, std::string geometryPath = "")
        {
            SGL_PROFILE_FUNCTION();
            this->vShaderFile = vertexPath;
            this->fShaderFile = fragmentPath;
            this->gShaderFile = geometryPath;
            std::string vertexCode;
            std::string fragmentCode;
            std::string geometryCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            std::ifstream gShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

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

                if (geometryPath != "")
                {
                    gShaderFile.open(geometryPath);
                    std::stringstream gShaderStream;
                    gShaderStream << gShaderFile.rdbuf();
                    gShaderFile.close();
                    geometryCode = gShaderStream.str();
                }
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
                GLchar vmessage[1024];
                GLCall(glGetShaderInfoLog(vertex, 1024, NULL, vmessage));
                std::cout << "\nVERTEX SHADER ERROR:\nFile: " << vertexPath << "\nProblem: "
                    << vmessage << std::endl;
            }
            // compile fragment shader
            GLCall(fragment = glCreateShader(GL_FRAGMENT_SHADER));
            GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
            GLCall(glCompileShader(fragment));
            GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &shaderCompileStatus));
            if (!shaderCompileStatus)
            {
                GLchar fmessage[1024];
                GLCall(glGetShaderInfoLog(fragment, 1024, NULL, fmessage));
                std::cout << "\nFRAGMENT SHADER ERROR:\nFile: " << fragmentPath << "\nProblem: " << fmessage << std::endl;
            }

            unsigned int geometry;
            if (geometryPath != "")
            {
                const char* gShaderCode = geometryCode.c_str();
                GLCall(geometry = glCreateShader(GL_GEOMETRY_SHADER));
                GLCall(glShaderSource(geometry, 1, &gShaderCode, NULL));
                GLCall(glCompileShader(geometry));

                GLCall(glGetShaderiv(geometry, GL_COMPILE_STATUS, &shaderCompileStatus));
                if (!shaderCompileStatus)
                {
                    GLchar fmessage[1024];
                    GLCall(glGetShaderInfoLog(geometry, 1024, NULL, fmessage));
                    std::cout << "\nGEOMETRY SHADER ERROR:\nFile: " << geometryPath << "\nProblem: " << fmessage << std::endl;
                }
            }

            // Create shader program
            GLCall(ID = glCreateProgram());
            GLCall(glAttachShader(ID, vertex));
            GLCall(glAttachShader(ID, fragment));
            if (geometryPath != "")
            {
                GLCall(glAttachShader(ID, geometry));
            }
            GLCall(glLinkProgram(ID));

            // check for linking errors
            GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &shaderCompileStatus));
            if (!shaderCompileStatus)
            {
                GLchar message[1024];
                GLCall(glGetProgramInfoLog(ID, 1024, NULL, message));
                std::cout << "\nLINKING SHADER ERROR: " << message << std::endl;
            }

            // Delete shaders as they have been linked to program and are no 
            // longer needed.
            GLCall(glDeleteShader(vertex));
            GLCall(glDeleteShader(fragment));
            if (geometryPath != "")
            {
                GLCall(glDeleteShader(geometry));
            }
        }

        void Destroy();

        void use();

        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        // --------------------------------------------------------------------
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;
        // --------------------------------------------------------------------
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        // --------------------------------------------------------------------
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w);
        // --------------------------------------------------------------------
        void setMat2(const std::string &name, const glm::mat2 &mat) const;
        // --------------------------------------------------------------------
        void setMat3(const std::string &name, const glm::mat3 &mat) const;
        // --------------------------------------------------------------------
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif
