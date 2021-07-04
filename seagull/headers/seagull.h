#ifndef SEAGULL_HEADER_FILE
#define SEAGULL_HEADER_FILE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include "shaders.h"
#include "vbo.h"
#include "Utils.h"

GLFWwindow *window;
int CurrentWidth = 640;
int CurrentHeight = 480;

double previousTime;
unsigned int FrameCount = 0;


GLuint
    ProjectionMatrixUniformLocation,
    ViewMatrixUniformLocation,
    ModelMatrixUniformLocation,
    ShadersIds[3];

Matrix
    ProjectionMatrix,
    ViewMatrix,
    ModelMatrix;
float rotationAngle = 0;
clock_t LastTime = 0;


void Initialize(void);
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif
