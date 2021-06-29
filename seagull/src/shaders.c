#include "shaders.h"

#include <GLFW//glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "errormsg.h"

GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId,
  VaoId,
  VboId;

char* read_shader(const char *path)
{
    FILE *fptr;
    if ((fptr = fopen(path, "r")) == NULL) {
        fprintf(stderr, "SHADER ERROR: %s could not be found!", path);
        exit(EXIT_FAILURE);
    }

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char *buffer = malloc(fsize + 1);
    fread(buffer, fsize, 1, fptr);
    fclose(fptr);
    buffer[fsize] = 0;

    char *result = "";
    result = malloc(512);
    memcpy(result, buffer, 512);

    return result;
}

void CreateVBO(void)
{
    Vertex Vertices[] = {
        { { -0.8f, -0.8f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { {  0.0f,  0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { {  0.8f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };
    
    const size_t BufferSize = sizeof(Vertices);
    const size_t VertexSize = sizeof(Vertices[0]);
    const size_t RgbOffset  = sizeof(Vertices[0].XYZW);

    GLCall(glGenVertexArrays(1, &VaoId));
    GLCall(glBindVertexArray(VaoId));

    GLCall(glGenBuffers(1, &VboId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VboId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0));
    GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glEnableVertexAttribArray(1));
}

void CreateShaders(const GLchar* VertexShader, const GLchar* FragmentShader)
{
    GLCall(VertexShaderId = glCreateShader(GL_VERTEX_SHADER));
    GLCall(glShaderSource(VertexShaderId, 1, &VertexShader, NULL));
    GLCall(glCompileShader(VertexShaderId));

    GLCall(FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
    GLCall(glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL));
    GLCall(glCompileShader(FragmentShaderId));

    GLCall(ProgramId = glCreateProgram());
    GLCall(glAttachShader(ProgramId, VertexShaderId));
    GLCall(glAttachShader(ProgramId, FragmentShaderId));
    GLCall(glLinkProgram(ProgramId));
    GLCall(glUseProgram(ProgramId));
}

void DestroyVBO(void)
{
    GLCall(glDisableVertexAttribArray(1));
    GLCall(glDisableVertexAttribArray(0));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &VboId));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &VaoId));
}

void DestroyShaders(void)
{
    GLCall(glUseProgram(0));
    GLCall(glDetachShader(ProgramId, VertexShaderId));
    GLCall(glDetachShader(ProgramId, FragmentShaderId));

    GLCall(glDeleteShader(VertexShaderId));
    GLCall(glDeleteShader(FragmentShaderId));

    GLCall(glDeleteProgram(ProgramId));
}

/*
 * Window Close Callback Func
 *
 * Destroys Shaders and Vertex Buffer Objects when window is closed
 */
void CleanUp(GLFWwindow *window)
{
    DestroyShaders();
    DestroyVBO();
}
