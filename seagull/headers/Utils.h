#ifndef UTILS_HEADER_FILE
#define UTILS_HEADER_FILE

#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _MSC_VER
#define VISUAL_STUDIO = true
#endif


static const double PI = 3.14159265358979323846;

int SubStrFind(char *text, char *search);


#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) if(!x) exit(-1);
#endif
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char *function, const char *file, int line);
void fix_render_on_mac(GLFWwindow *window);

#endif
