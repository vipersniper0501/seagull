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


int SubStrFind(char *text, char *search);


#ifdef _MSC_VER
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) if(!x) exit(-1);
#endif
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

inline void GLClearError(void)
{
    while (glGetError() != GL_NO_ERROR);
}

inline bool GLLogCall(const char *function, const char *file, int line)
{
    GLenum error = glGetError();
    while (error) {
        fprintf(stdout, "[OpenGL Error] (%d): %s %s: %d\n", error, function, file, line);
        return false;
    }
    return true;
}

/*
 * Fixes rendering problem on MacOS where rendering would start in bottom left
 * corner of screen and only fix once window was moved.
 */
inline void fix_render_on_mac(GLFWwindow *window)
{
#ifdef __APPLE__
    static bool macMoved = false;
    if(!macMoved) {
        int x, y;
        glfwGetWindowPos(window, &x, &y);
        glfwSetWindowPos(window, ++x, y);
        macMoved = true;
    }
#endif
}

#endif
