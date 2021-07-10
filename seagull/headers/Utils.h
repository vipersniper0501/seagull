#ifndef UTILS_HEADER_FILE
#define UTILS_HEADER_FILE

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


static const double PI = 3.14159265358979323846;

int SubStrFind(char *text, char *search);


#define ASSERT(x) if(!x) exit(-1);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char *function, const char *file, int line);
void fix_render_on_mac(GLFWwindow *window);

#endif
