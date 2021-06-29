#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include "errormsg.h"

#define WINDOW_NAME "Seagull"


GLFWwindow* window;
int CurrentWidth = 640;
int CurrentHeight = 480;

double previousTime;
unsigned int FrameCount = 0;


/*
 * Calculates FPS of engine
 */
static void timerFunction(GLFWwindow *window, double prevTime)
{
    double currentTime = glfwGetTime();
    FrameCount++;
    if (currentTime - prevTime >= 1.0) {
        char *TempString = (char*)
            malloc(512 + strlen(WINDOW_NAME));

        sprintf(
            TempString,
            "%s: %d Frames Per Second @ %d x %d",
            WINDOW_NAME,
            FrameCount,
            CurrentWidth,
            CurrentHeight
        );

        glfwSetWindowTitle(window, TempString);
        free(TempString);
        FrameCount = 0;
        previousTime = currentTime;
    }
}


/*
 * When window is resized, dimensions of OpenGL Viewport is set to the new
 * dimensions of the window.
 */
static void windowResizeCallbackFunc(GLFWwindow *window, int width, int height)
{
    CurrentWidth = width;
    CurrentHeight = height;
    GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));
}

/*
 * Error callback function for GLFW.
 *
 * If an error occurs, it will be printed with the number and description.
 */
static void GlfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW ERROR: [%d] %s\n", error, description);
}


/*
 * Creates a GLFW window ready for OpenGL
 * along with configuring some settings
 */
void InitWindow(void)
{
    /* Initialize the library */
    if (!glfwInit())
        exit(EXIT_FAILURE);


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, WINDOW_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    fprintf(stdout, "INFO: OpenGL Version: %s\n", glGetString(GL_VERSION));
    glfwSetWindowSizeCallback(window, windowResizeCallbackFunc);
    glfwSetErrorCallback(GlfwErrorCallback);
    glfwSetWindowCloseCallback(window, CleanUp);

    previousTime = glfwGetTime();
}

/*
 * Initializes a GLFW Window and GLEW along with some other settings.
 */
void Initialize(void)
{
    GLenum glewInitResult;

    InitWindow();

    glewExperimental = GL_TRUE;
    glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        fprintf(stdout, "GLEW ERROR: %s\n", glewGetErrorString(glewInitResult));
        exit(EXIT_FAILURE);
    }

    /*
     * NOTE: Shaders should be able to be dynamically loaded.
     * i.e. shader locations should not be hardcoded in.
     * Possible solution is make a standard directory to search for (shaders) and
     * locate all *.glsl files, and determine what they are. Once determined load
     * into variables/array
     */
    GLchar *vershdr;
    vershdr = read_shader("../../seagull/tmp_shaders/vertexShader.glsl");
    fprintf(stdout, "READ VERTEX SHADER:\n%s\n", vershdr);

    GLchar *fragshdr;
    fragshdr = read_shader("../../seagull/tmp_shaders/fragmentShader.glsl");
    fprintf(stdout, "READ FRAGMENT SHADER:\n%s\n", fragshdr);


    CreateShaders(vershdr, fragshdr);
    CreateVBO();

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
}

int main(void)
{

    Initialize();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        timerFunction(window, previousTime);


        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));




        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
