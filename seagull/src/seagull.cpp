/*
 *
 * This is the Seagull Game Engine main file.
 * Game loop information and Initialization configurations can be found here.
 *
 */

#include "seagull.h"
#include "glm/gtc/type_ptr.hpp"


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// #include <cglm/cglm.h>
#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC

#define WINDOW_NAME "Seagull"

GLFWwindow *window;
int CurrentWidth = 640;
int CurrentHeight = 480;

double previousTime;
unsigned int FrameCount = 0;

GLuint
    ProjectionMatrixUniformLocation,
    ViewMatrixUniformLocation,
    ModelMatrixUniformLocation,
    TransformMatrixUniformLocation,
    ShadersIds[3];

glm::mat4 
    ProjectionMatrix,
    ViewMatrix,
    ModelMatrix;

float rotationAngle = 0;
clock_t LastTime = 0;
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
                /* ActiveIndexBuffer = (ActiveIndexBuffer == 1 ? 0 : 1); */
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
    /* ProjectionMatrix =  */
        /* CreateProjectionMatrix( */
                /* 60, */
                /* (float)CurrentWidth/CurrentHeight, */
                /* 1.0f, */
                /* 100.0f); */
    // ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);
    /* GLCall(glUseProgram(ShadersIds[0])); */
    // mainShader.setMat4("ProjectionMatrix", ProjectionMatrix);
    // GLCall(glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, ProjectionMatrix[0]));
    /* GLCall(glUseProgram(0)); */
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
    // DestroyShaders(ShadersIds, 3);
    /* DestroyVBO(); */
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


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

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
    glfwSetFramebufferSizeCallback(window, windowResizeCallbackFunc);
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

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    // Useful for 3d objects
    /* GLCall(glEnable(GL_CULL_FACE)); */
    /* GLCall(glCullFace(GL_BACK)); */
    /* GLCall(glFrontFace(GL_CCW)); */

    fix_render_on_mac(window);
}

int main(void)
{

    Initialize();
    Shader mainShader("../../seagull/tmp_shaders/vertexShader.glsl", "../../seagull/tmp_shaders/fragmentShader.glsl");
    mainShader.use();

    // glm::mat4 ModelMatrix = glm::mat4(1.0f);
    // glm::mat4 ProjectionMatrix = glm::mat4(1.0f);
    glm::mat4 ViewMatrix = glm::mat4(1.0f);
    // glm_mat4_identity(ModelMatrix);
    // glm_mat4_identity(ProjectionMatrix);
    // glm_mat4_identity(ViewMatrix);
    // Moves the ViewMatrix to Z -2
    // glm_translate_z(ViewMatrix, -3);
    ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));


    // GLCall(ShadersIds[0] = glCreateProgram());
    // [> ShadersIds[1] = LoadShader("../../seagull/tmp_shaders/vertexShader.glsl", GL_VERTEX_SHADER); <]
    // ShadersIds[1] = LoadShader("../../seagull/tmp_shaders/vertexShader2.glsl", GL_VERTEX_SHADER);
    // ShadersIds[2] = LoadShader("../../seagull/tmp_shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER);
    // GLCall(glAttachShader(ShadersIds[0], ShadersIds[1]));
    // GLCall(glAttachShader(ShadersIds[0], ShadersIds[2]));
    // GLCall(glLinkProgram(ShadersIds[0]));
    // GLCall(glUseProgram(ShadersIds[0]));

    // GLCall(ModelMatrixUniformLocation = glGetUniformLocation(mainShader.ID, "ModelMatrix"));
    // GLCall(ViewMatrixUniformLocation = glGetUniformLocation(mainShader.ID, "ViewMatrix"));
    // GLCall(ProjectionMatrixUniformLocation = glGetUniformLocation(mainShader.ID, "ProjectionMatrix"));

    // GLCall(TransformMatrixUniformLocation = glGetUniformLocation(mainShader.ID, "transform"));

    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);
    mainShader.setMat4("ProjectionMatrix", ProjectionMatrix);


    // Mesh1.vertices_size = sizeof(Vertices)/sizeof(Vertices[0]);
    // Mesh1.indices_size = sizeof(Indices)/sizeof(Indices[0]);
    // Mesh1.indices = Indices;
    // for (int j = 0; j < Mesh1.indices_size; j++) {
        // fprintf(stdout, "Index [%d]: %d\n", j, Mesh1.indices[j]);
    // }

    // Vertex vertex;
    // vec3 vectorBuffer;
    // vec3 normalBuffervec;
    // vec2 texcoordsBuffervec;
    // MeshData tmp;

    // for (int i = 0; i < Mesh1.vertices_size; i++) {
        // vectorBuffer[0] = Vertices[i].Position[0];
        // vectorBuffer[1] = Vertices[i].Position[1];
        // vectorBuffer[2] = Vertices[i].Position[2];
        // normalBuffervec[0] = 0;
        // normalBuffervec[1] = 0;
        // normalBuffervec[2] = 0;
        // texcoordsBuffervec[0] = 0;
        // texcoordsBuffervec[1] = 0;
        // glm_vec3_copy(vectorBuffer, vertex.Position);
        // glm_vec4(vertex.Position, 1.0f, vertex.Position);
        // glm_vec3_copy(normalBuffervec, vertex.Normal);
        // glm_vec2_copy(texcoordsBuffervec, vertex.TexCoords);
        // tmp.vertices = realloc(Mesh1.vertices, sizeof(&Mesh1.vertices) + sizeof(Vertex));
        // Mesh1.vertices = tmp.vertices;
        // Mesh1.vertices[i] = vertex;
    // }
    // [> Everything has successfully been transferred to the struct!! :D <]

    // for (int i = 0; i < Mesh1.vertices_size; i++) {
        // fprintf(stdout, "Vertex [%d]: { %f, %f, %f, %f}\n", i, Mesh1.vertices[i].Position[0], Mesh1.vertices[i].Position[1], Mesh1.vertices[i].Position[2], Mesh1.vertices[i].Position[3]);
    // }


    Mesh crossMesh(Vertices, Indices, textures);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        timerFunction(window, previousTime);

        // activate shader
        // GLCall(glUseProgram(ShadersIds[0]));
        mainShader.use();

        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));

        glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);
        mainShader.setMat4("ProjectionMatrix", ProjectionMatrix);


        // Update shaders' uniforms
        mainShader.setMat4("ModelMatrix", ModelMatrix);
        mainShader.setMat4("ViewMatrix", ViewMatrix);


        // Draw Mesh
        crossMesh.Draw(mainShader);

        /* if (ActiveIndexBuffer == 0) { */
            /* GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[ActiveIndexBuffer])); */
            /* GLCall(glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL)); */
        /* } */
        /* else { */
            /* GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID[ActiveIndexBuffer])); */
            /* GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL)); */
        /* } */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    mainShader.Destroy();
    crossMesh.Destroy();

    glfwTerminate();
    std::cout << "Engine Stopped Successfully!" << std::endl;
    return 0;
}
