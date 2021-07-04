#ifndef SHADERS_HEADER_FILE
#define SHADERS_HEADER_FILE

#include "GL/glew.h"
#include "GLFW/glfw3.h"



GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId;

char* read_shader(const char *path);
GLuint LoadShader(const char *filename, GLenum shader_type);
void CreateShaders(const GLchar *VertexShader, const GLchar *FragmentShader);
void DestroyShaders(GLuint ShadersIds[], int size);

#endif
