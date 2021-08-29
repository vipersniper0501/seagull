#include "shaders.h"
#include "profiler.h"



void Shader::Destroy()
{
    SGL_PROFILE_FUNCTION();
    GLCall(glDeleteProgram(ID));
}

void Shader::use()
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUseProgram(ID));
}

void Shader::setBool(const std::string &name, bool value) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
}
void Shader::setInt(const std::string &name, int value) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
}
void Shader::setFloat(const std::string &name, float value) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
}
// --------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{ 
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0])); 
}
void Shader::setVec2(const std::string &name, float x, float y) const
{ 
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y)); 
}
// --------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0])); 
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{ 
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z)); 
}
// --------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w));
}
// --------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// --------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}
// --------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    SGL_PROFILE_FUNCTION();
    GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}




