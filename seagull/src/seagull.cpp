/*
 *
 * This is the Seagull Game Engine main file.
 * Game loop information and Initialization configurations can be found here.
 *
 */

#include "seagull.h"


#include <stdio.h>
#include <string>
#include <stdbool.h>
#include <stdlib.h>
#include <iostream>


#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE
#define GLEW_STATIC


Seagull::SeagullEngine seagull("Seagull");

int Seagull::SeagullEngine::CurrentWidth = 960;
int Seagull::SeagullEngine::CurrentHeight = 540;

double previousTime;

glm::vec3 lightPos;// = glm::vec3(-3.0f, 2.0f, -5.0f);

UI seagullUi;
Camera* Seagull::SeagullEngine::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float Seagull::SeagullEngine::lastX = CurrentWidth / 2.0f;
float Seagull::SeagullEngine::lastY = CurrentHeight / 2.0f;
bool Seagull::SeagullEngine::firstMouse = true;
bool Seagull::SeagullEngine::mouseControl = true;


// Cube Vertices
std::vector<Vertex> Vertices = {
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


std::vector<unsigned int> Indices = {
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

std::vector<Texture>textures = {};


void updateLighting(Shader &shader, glm::vec3 lightPos)
{
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("lighting.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    shader.setVec3("lighting.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    shader.setVec3("lighting.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    shader.setFloat("lighting.constant", seagullUi.lampIntensity); //light intensity (lower=brighter)
    shader.setFloat("lighting.linear", 0.045f);
    shader.setFloat("lighting.quadratic", 0.0075f);
}


int main(void)
{

    /*
     * This view matrix pushes everything back by 3 units before first draw,
     * works the same as pushing the camera back by 3 units.
     */ 
    glm::mat4 ViewMatrix = seagull.camera->GetViewMatrix();
    glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)seagull.CurrentWidth/seagull.CurrentHeight, 1.0f, 100.0f);

    // Could have a camera.update() function to loop through loaded shaders and update the uniforms with new values to reduce
    // duplicated code.

    Shader EyeballShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"));
    Model EyeballModel(FileSystem::getPath("seagull/tmp/Eyeball/Eyeball.gltf"));
    SceneInfo.loaded_models.push_back(EyeballModel);
    EyeballShader.use();
    EyeballShader.setMat4("ViewMatrix", ViewMatrix);
    EyeballShader.setMat4("ProjectionMatrix", ProjectionMatrix);


    Shader HelmetShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"));
    Model HelmetModel(FileSystem::getPath("seagull/tmp/helmet/DamagedHelmet.gltf"));
    SceneInfo.loaded_models.push_back(HelmetModel);
    HelmetShader.use();
    HelmetShader.setMat4("ViewMatrix", ViewMatrix);
    HelmetShader.setMat4("ProjectionMatrix", ProjectionMatrix);


    Shader backpackShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"));
    Model backpackModel(FileSystem::getPath("seagull/tmp/backpack/backpack.obj"));
    SceneInfo.loaded_models.push_back(backpackModel);
    backpackShader.use();
    backpackShader.setMat4("ViewMatrix", ViewMatrix);
    backpackShader.setMat4("ProjectionMatrix", ProjectionMatrix);

    // Shader cityShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"));
    // Model cityModel(FileSystem::getPath("seagull/tmp/ugly_city.fbx"));
    // SceneInfo.loaded_models.push_back(cityModel);
    // cityShader.use();
    // cityShader.setMat4("ViewMatrix", ViewMatrix);
    // cityShader.setMat4("ProjectionMatrix", ProjectionMatrix);

    Shader lightCubeShader(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));
    Mesh cubeMesh(Vertices, Indices, textures);
    lightCubeShader.use();
    lightCubeShader.setMat4("ViewMatrix", ViewMatrix);
    lightCubeShader.setMat4("ProjectionMatrix", ProjectionMatrix);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(seagull.window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

        double currentFrame = glfwGetTime();
        seagull.deltaTime = currentFrame - seagull.lastFrame;
        seagull.lastFrame = currentFrame;

        seagull.ProcessInput();



        lightPos = glm::make_vec3(seagullUi.lampLocation);

        // activate shader
        backpackShader.use();

        // Set/Update ProjectionMatrix
        ViewMatrix = seagull.camera->GetViewMatrix();
        ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)seagull.CurrentWidth/seagull.CurrentHeight, 1.0f, 100.0f);

        // // Transform Mesh around center
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3( 0.0f, 0.0f, -9.0f));
        ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0, 0.1, 0.0));


        // // Update shaders' uniforms
        backpackShader.setMat4("ViewMatrix", ViewMatrix);
        backpackShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        backpackShader.setMat4("ModelMatrix", ModelMatrix);


        // // Backpack Model's lighting settings
        backpackShader.setFloat("material.shininess", 64.0f);
        backpackShader.setVec3("viewPos", seagull.camera->Position);

        updateLighting(backpackShader, lightPos);

        // // Draw Backpack Mesh
        backpackModel.Draw(backpackShader);





        EyeballShader.use();

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.0f, -2.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(100.0f, 100.0f, 100.0f));

        EyeballShader.setMat4("ViewMatrix", ViewMatrix);
        EyeballShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        EyeballShader.setMat4("ModelMatrix", ModelMatrix);

        EyeballShader.setFloat("material.shininess", 64.0f);
        EyeballShader.setVec3("viewPos", seagull.camera->Position);

        updateLighting(EyeballShader, lightPos);

        // Draw Eyeball Mesh
        EyeballModel.Draw(EyeballShader);



        HelmetShader.use();

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-2.0f, 1.0f, -3.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        HelmetShader.setMat4("ViewMatrix", ViewMatrix);
        HelmetShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        HelmetShader.setMat4("ModelMatrix", ModelMatrix);

        HelmetShader.setFloat("material.shininess", 64.0f);
        HelmetShader.setVec3("viewPos", seagull.camera->Position);

        updateLighting(HelmetShader, lightPos);

        // Draw Monolith Mesh
        HelmetModel.Draw(HelmetShader);


        lightCubeShader.use();

        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, lightPos);
        // ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        lightCubeShader.setMat4("ViewMatrix", ViewMatrix);
        lightCubeShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        lightCubeShader.setMat4("ModelMatrix", ModelMatrix);

        // Draw LightCube Mesh
        cubeMesh.Draw(lightCubeShader);

        seagullUi.NewFrame();
        seagullUi.ShowMainMenuBar();

        seagullUi.RenderUi();
        /* Swap front and back buffers */
        glfwSwapBuffers(seagull.window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    backpackShader.Destroy();
    lightCubeShader.Destroy();
    EyeballShader.Destroy();

    seagullUi.Destroy();

    glfwTerminate();
    return 0;
}
