#include "shaders.h"

#include <GLFW//glfw3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Utils.h"

GLint shaderCompileStatus;

GLuint
  VertexShaderId,
  FragmentShaderId,
  ProgramId;

GLuint LoadShader(const char *filename, GLenum shader_type)
{
    GLuint shader_id = 0;
    FILE *file;
    int file_size = -1;
    char *glsl_data;

    if (NULL != (file = fopen(filename, "rb")) &&
        0 == fseek(file, 0, SEEK_END) &&
        -1 != (file_size = ftell(file)))
    {
        rewind(file);
        
        if (NULL != (glsl_data = (char*)malloc(file_size + 1))) {

            if (file_size == (long)fread(glsl_data, sizeof(char), file_size, file)) {

                glsl_data[file_size] = '\0';
                GLCall(shader_id = glCreateShader(shader_type));

                const GLchar *glsl_source = glsl_data;
                if (0 != shader_id) {
                    GLCall(glShaderSource(shader_id, 1, &glsl_source, NULL));
                    GLCall(glCompileShader(shader_id));
                    GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shaderCompileStatus));
                    if (shaderCompileStatus != GL_TRUE) {
                        GLsizei log_length = 0;
                        GLchar message[1024];
                        GLCall(glGetShaderInfoLog(shader_id, 1024, &log_length, message));
                        fprintf(stderr, "SHADER ERROR: \nFile: %s\nProblem: %s\n", filename, message);
                    }
                } else
                    fprintf(stderr, "ERROR: Could not create a shader.\n");
            } else
                fprintf(stderr, "ERROR: Could not read file %s\n", filename);

            free(glsl_data);
        } else
            fprintf(stderr, "ERROR: Could not allocate %i bytes.\n", file_size);

        fclose(file);
    } else {
        if (NULL != file)
            fclose(file);
        fprintf(stderr, "ERROR: Could not open file %s\n", filename);
    }
    return shader_id;
}

/*
 * Read shader data from a *.glsl file and return the shader information
 */
// char* read_shader(const char *path)
// {
    // FILE *fptr;
    // if ((fptr = fopen(path, "r")) == NULL) {
        // fprintf(stderr, "SHADER ERROR: %s could not be found!", path);
        // exit(EXIT_FAILURE);
    // }

    // fseek(fptr, 0, SEEK_END);
    // long fsize = ftell(fptr);
    // fseek(fptr, 0, SEEK_SET);
    // char *buffer = malloc(fsize + 1);
    // fread(buffer, fsize, 1, fptr);
    // fclose(fptr);
    // buffer[fsize] = 0;

    // char *result = "";
    // result = malloc(512);
    // memcpy(result, buffer, 512);

    // return result;
// }

/*
void CreateShaders(const GLchar *VertexShader, const GLchar *FragmentShader)
 */
void CreateShaders(const GLchar *VertexShader, const GLchar *FragmentShader)
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

/*
 * Detaches and Deletes shaders from program and deletes program
 */
void DestroyShaders(GLuint ShadersIds[], int size)
{
    GLCall(glUseProgram(ShadersIds[0]));
    for (int i = 1; i < size; i++) {
        GLCall(glDetachShader(ShadersIds[0], ShadersIds[i]));
        GLCall(glDeleteShader(ShadersIds[i]));
    }
    /* GLCall(glDetachShader(ProgramId, VertexShaderId)); */
    /* GLCall(glDetachShader(ProgramId, FragmentShaderId)); */

    /* GLCall(glDeleteShader(VertexShaderId)); */
    /* GLCall(glDeleteShader(FragmentShaderId)); */

    GLCall(glDeleteProgram(ShadersIds[0]));
}

