/*
 *
 * This is the Seagull Game Engine main file.
 * Game loop information and Initialization configurations can be found here.
 *
 */

#include "seagull.h"
#include "glm/gtc/type_ptr.hpp"
#include "filesystem.h"


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>


#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC

#define WINDOW_NAME "Seagull"

GLFWwindow *window;
int CurrentWidth = 960;
int CurrentHeight = 540;

double previousTime;
unsigned int FrameCount = 0;

GLuint
    ProjectionMatrixUniformLocation,
    ViewMatrixUniformLocation,
    ModelMatrixUniformLocation;

glm::mat4 
    ProjectionMatrix,
    ViewMatrix,
    ModelMatrix;

glm::vec3 lightPos;// = glm::vec3(-3.0f, 2.0f, -5.0f);

UI seagullUi;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = CurrentWidth / 2.0f;
float lastY = CurrentHeight / 2.0f;
bool firstMouse = true;
bool mouseControl = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// Cube Vertices
vector<Vertex> Vertices = {
    // front
    {{-0.5, -0.5,  0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5, -0.5,  0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5,  0.5,  0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-0.5,  0.5,  0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    // back
    {{-0.5, -0.5, -0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5, -0.5, -0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{ 0.5,  0.5, -0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{-0.5,  0.5, -0.5}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
};


vector<unsigned int> Indices = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};

vector<Texture>textures = {};



/*
 * @Brief: Processes user keypress
 * This function takes user input and assigns actions to the keypresses.
 * This is useful for when a key should only be activated once while pressed down.
 *
 * NOTE: In future, make it so that this can be easily modified.
 */
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                mouseControl = (mouseControl == false ? true : false);
                if (!mouseControl)
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                else
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;

            default:
                break;
        }
    }
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseControl)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
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
    window = glfwCreateWindow(CurrentWidth, CurrentHeight, WINDOW_NAME, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    fprintf(stdout, "INFO: \n"
                    "OpenGL Version: %s\n"
                    "GLFW Version: %s\n"
                    "Dear ImGui Version: %s\n"
                    "Assimp: %u.%u.%u\n", 
                    glGetString(GL_VERSION), glfwGetVersionString(), ImGui::GetVersion(),
                    aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch());
    glfwSetFramebufferSizeCallback(window, windowResizeCallbackFunc);
    glfwSetErrorCallback(GlfwErrorCallback);
    glfwSetKeyCallback(window, processKeypress);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    previousTime = glfwGetTime();
}

/*
 * Initializes a GLFW Window and GLEW along with some basic OpenGL settings.
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

    seagullUi.init(window);

    GLCall(glViewport(0, 0, CurrentWidth, CurrentHeight));

    stbi_set_flip_vertically_on_load(true);

    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glDepthFunc(GL_LESS));

    GLCall(glEnable(GL_STENCIL_TEST));

    fix_render_on_mac(window);
}

int main(void)
{

    Initialize();

    /*
     * This view matrix pushes everything back by 3 units before first draw,
     * works the same as pushing the camera back by 3 units.
     */ 
    glm::mat4 ViewMatrix = camera.GetViewMatrix();
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);

    // Could have a camera.update() function to loop through loaded shaders and update the uniforms with new values to reduce
    // duplicated code.

    Shader backpackShader("../../seagull/tmp_shaders/backpackVertex.glsl", "../../seagull/tmp_shaders/backpackFragment.glsl");
    Model backpackModel(FileSystem::getPath("seagull/tmp/backpack/backpack.obj"));
    backpackShader.use();
    backpackShader.setMat4("ViewMatrix", ViewMatrix);
    backpackShader.setMat4("ProjectionMatrix", ProjectionMatrix);
    backpackShader.setInt("material.diffuse", 0);
    backpackShader.setInt("material.specular", 1);


    Shader lightCubeShader("../../seagull/tmp_shaders/lightCubeVertex.glsl", "../../seagull/tmp_shaders/lightCubeFragment.glsl");
    Mesh cubeMesh(Vertices, Indices, textures);
    lightCubeShader.use();
    lightCubeShader.setMat4("ViewMatrix", ViewMatrix);
    lightCubeShader.setMat4("ProjectionMatrix", ProjectionMatrix);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        seagullUi.NewFrame();
        seagullUi.ShowMainMenuBar();


        lightPos = glm::make_vec3(seagullUi.lampLocation);

        // activate shader
        backpackShader.use();

        // Set/Update ProjectionMatrix
        ViewMatrix = camera.GetViewMatrix();
        glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)CurrentWidth/CurrentHeight, 1.0f, 100.0f);

        // Transform Mesh around center
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3( 0.0f, 0.0f, -9.0f));
        ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0, 0.1, 0.0));


        // Update shaders' uniforms
        backpackShader.setMat4("ViewMatrix", ViewMatrix);
        backpackShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        backpackShader.setMat4("ModelMatrix", ModelMatrix);


        // Backpack Model's lighting settings
        backpackShader.setFloat("material.shininess", 64.0f);
        backpackShader.setVec3("viewPos", camera.Position); // viewPos should be set to camera.Position when camera is in use.
        backpackShader.setVec3("pointLight.position", lightPos);
        backpackShader.setVec3("pointLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        backpackShader.setVec3("pointLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        backpackShader.setVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        backpackShader.setFloat("pointLight.constant", seagullUi.lampIntensity); //light intensity (lower=brighter)
        backpackShader.setFloat("pointLight.linear", 0.045f);
        backpackShader.setFloat("pointLight.quadratic", 0.0075f);

        // Draw Backpack Mesh
        backpackModel.Draw(backpackShader);




        lightCubeShader.use();

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, lightPos);
        // ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        lightCubeShader.setMat4("ViewMatrix", ViewMatrix);
        lightCubeShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        lightCubeShader.setMat4("ModelMatrix", ModelMatrix);

        // Draw LightCube Mesh
        cubeMesh.Draw(lightCubeShader);




        seagullUi.RenderUi();
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    backpackShader.Destroy();
    lightCubeShader.Destroy();

    seagullUi.Destroy();

    glfwTerminate();
    return 0;
}
