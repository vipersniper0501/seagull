/*
 *
 * This is the Seagull Game Engine main file.
 * Game loop information and Initialization configurations can be found here.
 *
 */

#include "seagull.h"


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC

#define WINDOW_NAME "Seagull"


/*
 * @Brief: Processes user keypress
 * This function takes user input and assigns actions to the keypresses.
 *
 * NOTE: In future, make it so that this can be easily modified.
 */
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            // Temporary case.
            case GLFW_KEY_T:
                ActiveIndexBuffer = (ActiveIndexBuffer == 1 ? 0 : 1);
                /* GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[ActiveIndexBuffer])); */
                break;

            default:
                break;
        }
    }
}

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
void windowResizeCallbackFunc(GLFWwindow *window, int width, int height)
{
    CurrentWidth = width;
    CurrentHeight = height;
    GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));
    ProjectionMatrix = 
        CreateProjectionMatrix(
                60,
                (float)CurrentWidth/CurrentHeight,
                1.0f,
                100.0f);
    GLCall(glUseProgram(ShadersIds[0]));
    GLCall(glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix.m));
    GLCall(glUseProgram(0));
}

/*
 * Error callback function for GLFW.
 *
 * If an error occurs, it will be printed with the number and description.
 */
void GlfwErrorCallback(int error, const char *description)
{
    fprintf(stderr, "GLFW ERROR: [%d] %s\n", error, description);
}

/*
 * Window Close Callback Func
 *
 * Destroys Shaders and Vertex Buffer Objects when window is closed
 */
void CleanUp(GLFWwindow *window)
{
    DestroyShaders(ShadersIds, 3);
    DestroyVBO();
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
    glfwSetKeyCallback(window, processKeypress);

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

    GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));

    /* GLCall(glEnable(GL_DEBUG_OUTPUT)); */

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // Useful for 3d objects
    /* GLCall(glEnable(GL_CULL_FACE)); */
    /* GLCall(glCullFace(GL_BACK)); */
    /* GLCall(glFrontFace(GL_CCW)); */

    ModelMatrix = IDENTITY_MATRIX;
    ProjectionMatrix = IDENTITY_MATRIX;
    ViewMatrix = IDENTITY_MATRIX;
    // Moves the ViewMatrix to Z -2
    TranslateMatrix(&ViewMatrix, 0, 0, -2);

    GLCall(ShadersIds[0] = glCreateProgram());
    ShadersIds[1] = LoadShader("../../seagull/tmp_shaders/vertexShader.glsl", GL_VERTEX_SHADER);
    ShadersIds[2] = LoadShader("../../seagull/tmp_shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
    GLCall(glAttachShader(ShadersIds[0], ShadersIds[1]));
    GLCall(glAttachShader(ShadersIds[0], ShadersIds[2]));
    GLCall(glLinkProgram(ShadersIds[0]));

    GLCall(ModelMatrixUniformLocation = glGetUniformLocation(ShadersIds[0], "ModelMatrix"));
    GLCall(ViewMatrixUniformLocation = glGetUniformLocation(ShadersIds[0], "ViewMatrix"));
    GLCall(ProjectionMatrixUniformLocation = glGetUniformLocation(ShadersIds[0], "ProjectionMatrix"));

    CreateVBO();
}

int main(void)
{

    Initialize();


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        timerFunction(window, previousTime);



        float CurrentAngle;
        clock_t Now = clock();
        if (LastTime == 0)
            LastTime = Now;

        rotationAngle += 45.0f * (float)(Now - LastTime) / CLOCKS_PER_SEC;
        CurrentAngle = DegreesToRadians(rotationAngle);
        ModelMatrix = IDENTITY_MATRIX;
        RotateAboutZ(&ModelMatrix, CurrentAngle);

        // Use updated shaders
        GLCall(glUseProgram(ShadersIds[0]));


        // Update shaders' uniforms
        GLCall(glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.m));
        GLCall(glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.m));


        // Use updated VertexArray
        GLCall(glBindVertexArray(BufferIDs[0]));


        if (ActiveIndexBuffer == 0) {
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[ActiveIndexBuffer]));
            GLCall(glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL));
        }
        else {
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[ActiveIndexBuffer]));
            GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL));
        }

        /*
         * Stop using VertexArray and shaders so they can be updated on next
         * cycle
         */
        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));


        LastTime = Now;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
