#ifndef SHADERS_HEADER_FILE
#define SHADERS_HEADER_FILE

#include "GL/glew.h"
#include "GLFW/glfw3.h"

typedef struct
{
    float XYZW[4];
    float RGBA[4];
} Vertex;

char* read_shader(const char *path);
void CreateVBO(void);
void CreateShaders(const GLchar* VertexShader, const GLchar* FragmentShader);
void DestroyVBO(void);
void DestroryShaders(void);
void CleanUp(GLFWwindow *window);

#endif
