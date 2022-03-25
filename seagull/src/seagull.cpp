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

glm::vec3 lightPos[64];

UI seagullUi;
Camera* Seagull::SeagullEngine::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float Seagull::SeagullEngine::lastX = CurrentWidth / 2.0f;
float Seagull::SeagullEngine::lastY = CurrentHeight / 2.0f;
bool Seagull::SeagullEngine::firstMouse = true;
bool Seagull::SeagullEngine::mouseControl = false;


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



class Game : Seagull::SeagullEngine
{

    public:

        void Init()
        {

        }

        void OnUpdate(float deltaTime)
        {

        }

        void OnRender()
        {

        }

        void OnImGuiRender()
        {
            seagullUi.NewFrame();

            seagullUi.ShowMainMenuBar();

            seagullUi.RenderUi();
        }

    private:

        void updateLighting(Shader &shader, glm::vec3 lightPos, Model model)
        {
            for(unsigned int i = 0; i <= model.nrLights; i++)
            {
                shader.setVec3("lightPos", lightPos);
                shader.setVec3("lighting[" + std::to_string(i) + "].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
                shader.setVec3("lighting" + std::to_string(i) + ".diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
                shader.setVec3("lighting" + std::to_string(i) + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));

                shader.setFloat("lighting" + std::to_string(i) + ".constant", seagullUi.lampIntensity); //light intensity (lower=brighter)
                shader.setFloat("lighting" + std::to_string(i) + ".linear", 0.045f);
                shader.setFloat("lighting" + std::to_string(i) + ".quadratic", 0.0075f);
            }
        }

};












/*
    Imported lights are not really working right now. They are not being affected by its parents model matrix as it becomes its own seperate
    entity when read in. (its location values are ripped from the file and are no longer affected by the rest of the model)
*/
void updateLighting(Shader &shader, glm::vec3 *lightPos, unsigned int nrLights)
{
    SGL_PROFILE_FUNCTION();
    shader.setInt("nrLights", nrLights);
    for(unsigned int i = 0; i < nrLights; i++)
    {
        shader.setVec3("lightPos[" + std::to_string(i) + "]", lightPos[i]);

        shader.setVec3("lighting[" + std::to_string(i) + "].ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("lighting[" + std::to_string(i) + "].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("lighting[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // attenuation
        shader.setFloat("lighting[" + std::to_string(i) + "].constant", seagullUi.lampIntensity);
        shader.setFloat("lighting[" + std::to_string(i) + "].linear", 0.045f);
        shader.setFloat("lighting[" + std::to_string(i) + "].quadratic", 0.0075f);
    }
}

int main(void)
{

    /*
     * This view matrix pushes everything back by 3 units before first draw,
     * works the same as pushing the camera back by 3 units.
     */ 
    glm::mat4 ViewMatrix;
    glm::vec3 ViewPos;
    glm::mat4 ProjectionMatrix;
    glm::mat4 ModelMatrix;


    // Load Models and Shaders
    /*Shader EyeballShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"), "EyeballShader");
    Model EyeballModel = ResourceManager::LoadModel(FileSystem::getPath("seagull/tmp/Eyeball/Eyeball.gltf"), "EyeballModel");*/
    Shader HelmetShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"), "HelmetShader");
    Model HelmetModel = ResourceManager::LoadModel(FileSystem::getPath("seagull/tmp/helmet/DamagedHelmet.gltf"), "HelmetModel");
    Shader backpackShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"), "backpackShader");
    Model backpackModel = ResourceManager::LoadModel(FileSystem::getPath("seagull/tmp/backpack/backpack.obj"), "backpackModel");
    //Shader CityShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"), "CityShader");
    //Model CityModel = ResourceManager::LoadModel(FileSystem::getPath("seagull/tmp/ugly_city.gltf"), "CityModel");
    Shader lightCubeShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"), "lightCubeShader");
    /*Shader lightCubeShader2(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));
    Shader lightCubeShader3(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));
    Shader lightCubeShader4(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));
    Shader lightCubeShader5(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));
    Shader lightCubeShader6(FileSystem::getPath("seagull/tmp_shaders/lightCubeVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/lightCubeFragment.glsl"));*/
    Mesh cubeMesh(Vertices, Indices, textures, 64.0f);

    Shader PhilShaderNormals = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/normalsVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/normalsFragment.glsl"), FileSystem::getPath("seagull/tmp_shaders/normalsGeometry.glsl"), "PhilShaderNormals");
    Shader PhilShader = ResourceManager::LoadShader(FileSystem::getPath("seagull/tmp_shaders/defaultVertex.glsl"), FileSystem::getPath("seagull/tmp_shaders/defaultFragment.glsl"), "PhilShader");
    Model PhilModel = ResourceManager::LoadModel(FileSystem::getPath("seagull/tmp/Phil.gltf"), "PhilModel");

    Seagull::Object Phil;
    Phil.shader = PhilShader;
    Phil.model = PhilModel;





 
    //unsigned int nrLights = ResourceManager::GetNumberOfLights();
    std::vector<aiLight> lights = ResourceManager::GetLights();
    lightPos[0] = glm::make_vec3(seagullUi.lampLocation);
    for (unsigned int i = 0; i < lights.size(); i++)
    {
        // convert assimp light coordinates to glm vec3 coordinates
        lightPos[i+1].x = lights[i].mPosition.x;
        lightPos[i+1].y = lights[i].mPosition.y;
        lightPos[i+1].z = lights[i].mPosition.z;
        std::cout << "Light Position: Index " << std::to_string(i+1) << ": " << std::to_string(lightPos[i+1].x) <<
                                                                        ", " << std::to_string(lightPos[i+1].y) <<
                                                                        ", " << std::to_string(lightPos[i+1].z) << std::endl; 
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(seagull.window))
    {
        SGL_PROFILE_MAIN();
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        double currentFrame = glfwGetTime();
        seagull.deltaTime = currentFrame - seagull.lastFrame;
        seagull.lastFrame = currentFrame;

        seagull.ProcessInput();

        ViewMatrix = seagull.camera->GetViewMatrix();
        ResourceManager::SaveMatrix(ViewMatrix, "ViewMatrix");
        ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)seagull.CurrentWidth/seagull.CurrentHeight, 0.1f, 100.0f);
        ResourceManager::SaveMatrix(ProjectionMatrix, "ProjectionMatrix");
        ViewPos = seagull.camera->Position;
        ResourceManager::SaveVec3(ViewPos, "ViewPos");


        // Update
        lightPos[0] = glm::make_vec3(seagullUi.lampLocation);

        backpackShader.use();
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3( 0.0f, 0.0f, -9.0f));
        ModelMatrix = glm::rotate(ModelMatrix, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(0.0, 0.1, 0.0));
        backpackShader.setMat4("ViewMatrix", ViewMatrix);
        backpackShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        backpackShader.setMat4("ModelMatrix", ModelMatrix);
        // Backpack Model's lighting settings
        backpackShader.setFloat("material.shininess", 64.0f);
        backpackShader.setVec3("viewPos", ViewPos);
        //updateLighting(backpackShader, lightPos, lights.size());
        updateLighting(backpackShader, lightPos, 1);




        //EyeballShader.use();
        //ModelMatrix = glm::mat4(1.0f);
        //ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -1.0f, -2.5f));
        //EyeballShader.setMat4("ViewMatrix", ViewMatrix);
        //EyeballShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        //EyeballShader.setMat4("ModelMatrix", ModelMatrix);
        //EyeballShader.setFloat("material.shininess", 64.0f);
        //EyeballShader.setVec3("viewPos", ViewPos);
        ////updateLighting(EyeballShader, lightPos, lights.size());
        //updateLighting(EyeballShader, lightPos, 1);


        HelmetShader.use();
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-2.0f, 1.0f, -3.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        ResourceManager::SaveMatrix(ModelMatrix, HelmetModel.name);
        HelmetShader.setMat4("ViewMatrix", ViewMatrix);
        HelmetShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        HelmetShader.setMat4("ModelMatrix", ModelMatrix);
        HelmetShader.setFloat("material.shininess", 64.0f);
        HelmetShader.setVec3("viewPos", ViewPos);
        //updateLighting(HelmetShader, lightPos, lights.size());
        updateLighting(HelmetShader, lightPos, 1);


        //CityShader.use();
        //ModelMatrix = glm::mat4(1.0f);
        //ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, -2.0f, 0.0f));
        //ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //CityShader.setMat4("ViewMatrix", ViewMatrix);
        //CityShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        //CityShader.setMat4("ModelMatrix", ModelMatrix);
        //CityShader.setFloat("material.shininess", 64.0f);
        //CityShader.setVec3("viewPos", ViewPos);
        //updateLighting(CityShader, lightPos, lights.size());



        Phil.shader.use();
        Phil.setScale(glm::vec3(0.25, 0.25, 0.25));
        ResourceManager::SaveMatrix(Phil.getModelMatrix(), PhilModel.name);
        Phil.shader.setFloat("material.shininess", 64.0f);
        updateLighting(Phil.shader, lightPos, 1);


        lightCubeShader.use();
        ModelMatrix = glm::mat4(1.0f);
        ModelMatrix = glm::translate(ModelMatrix, lightPos[0]);
        lightCubeShader.setMat4("ViewMatrix", ViewMatrix);
        lightCubeShader.setMat4("ProjectionMatrix", ProjectionMatrix);
        lightCubeShader.setMat4("ModelMatrix", ModelMatrix);


        /*lightCubeShader2.use();
         ModelMatrix = glm::mat4(1.0f);
         ModelMatrix = glm::translate(ModelMatrix, lightPos[1]);
         lightCubeShader2.setMat4("ViewMatrix", ViewMatrix);
         lightCubeShader2.setMat4("ProjectionMatrix", ProjectionMatrix);
         lightCubeShader2.setMat4("ModelMatrix", ModelMatrix);


         lightCubeShader3.use();
         ModelMatrix = glm::mat4(1.0f);
         ModelMatrix = glm::translate(ModelMatrix, lightPos[2]);
         lightCubeShader3.setMat4("ViewMatrix", ViewMatrix);
         lightCubeShader3.setMat4("ProjectionMatrix", ProjectionMatrix);
         lightCubeShader3.setMat4("ModelMatrix", ModelMatrix);


         lightCubeShader4.use();
         ModelMatrix = glm::mat4(1.0f);
         ModelMatrix = glm::translate(ModelMatrix, lightPos[3]);
         lightCubeShader4.setMat4("ViewMatrix", ViewMatrix);
         lightCubeShader4.setMat4("ProjectionMatrix", ProjectionMatrix);
         lightCubeShader4.setMat4("ModelMatrix", ModelMatrix);


         lightCubeShader5.use();
         ModelMatrix = glm::mat4(1.0f);
         ModelMatrix = glm::translate(ModelMatrix, lightPos[4]);
         lightCubeShader5.setMat4("ViewMatrix", ViewMatrix);
         lightCubeShader5.setMat4("ProjectionMatrix", ProjectionMatrix);
         lightCubeShader5.setMat4("ModelMatrix", ModelMatrix);


         lightCubeShader6.use();
         ModelMatrix = glm::mat4(1.0f);
         ModelMatrix = glm::translate(ModelMatrix, lightPos[5]);
         lightCubeShader6.setMat4("ViewMatrix", ViewMatrix);
         lightCubeShader6.setMat4("ProjectionMatrix", ProjectionMatrix);
         lightCubeShader6.setMat4("ModelMatrix", ModelMatrix);*/



        // Render
        backpackShader.use();
        backpackModel.Draw(backpackShader);
        // EyeballShader.use();
        // EyeballModel.Draw(EyeballShader);
        HelmetShader.use();
        HelmetModel.Draw(HelmetShader);
       /*CityShader.use();
        CityModel.Draw(CityShader);*/
        lightCubeShader.use();
        cubeMesh.Draw(lightCubeShader);
       /*lightCubeShader2.use();
        cubeMesh.Draw(lightCubeShader);
        lightCubeShader3.use();
        cubeMesh.Draw(lightCubeShader);
        lightCubeShader4.use();
        cubeMesh.Draw(lightCubeShader);
        lightCubeShader5.use();
        cubeMesh.Draw(lightCubeShader);
        lightCubeShader6.use();
        cubeMesh.Draw(lightCubeShader);*/
        Phil.shader.use();
        PhilModel.Draw(Phil.shader);

        seagullUi.NewFrame();
        seagullUi.ShowMainMenuBar();
        seagullUi.RenderUi();
        /* Swap front and back buffers */
        glfwSwapBuffers(seagull.window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    //backpackShader.Destroy();
    //lightCubeShader.Destroy();
    /*EyeballShader.Destroy();
    HelmetShader.Destroy();*/

    seagullUi.Destroy();

    glfwTerminate();
    return 0;
}
