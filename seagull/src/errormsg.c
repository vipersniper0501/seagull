#include "errormsg.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdbool.h>


void GLClearError(void)
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char *function, const char *file, int line)
{
    GLenum error = glGetError();
    while (error) {
        fprintf(stdout, "[OpenGL Error] (%d): %s %s: %d\n", error, function, file, line);
        return false;
    }
    return true;
}
