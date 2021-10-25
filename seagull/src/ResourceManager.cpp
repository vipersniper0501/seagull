#include "ResourceManager.h"
#include "profiler.h"


std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
std::map<std::string, glm::mat4> ResourceManager::Matrices;
std::map<std::string, glm::vec3> ResourceManager::VecThrees;

Shader ResourceManager::LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name)
{
    SGL_PROFILE_FUNCTION();
    Shader shader_tmp(vShaderFile, fShaderFile);
    Shaders[name] = shader_tmp;
    return Shaders[name];
}

Shader ResourceManager::LoadShader(std::string vShaderFile, std::string fShaderFile, std::string gShaderFile, std::string name)
{
    SGL_PROFILE_FUNCTION();
    if (Shaders.count(name) == 0)
    {
        Shader shader_tmp(vShaderFile, fShaderFile, gShaderFile);
        Shaders[name] = shader_tmp;
        return Shaders[name];
    }
    else
    {
        return Shaders[name];
    }
}

Shader ResourceManager::GetShader(std::string name)
{
    SGL_PROFILE_FUNCTION();
    return Shaders[name];
}

Model ResourceManager::LoadModel(std::string ModelFileLocation, std::string name)
{
    SGL_PROFILE_FUNCTION();
    Model model_tmp(ModelFileLocation);
    Models[name] = model_tmp;
    return Models[name];
}

Model ResourceManager::GetModel(std::string name)
{
    SGL_PROFILE_FUNCTION();
    return Models[name];
}

unsigned int ResourceManager::GetNumberOfLights()
{
    SGL_PROFILE_FUNCTION();
    unsigned int lights = 0;
    for(auto i = Models.begin(); i != Models.end(); i++)
    {
        lights += i->second.nrLights;
    }
    return lights;
}

std::vector<aiLight> ResourceManager::GetLights()
{
    SGL_PROFILE_FUNCTION();
    std::vector<aiLight> lights;
    for (auto i = Models.begin(); i != Models.end(); i++)
    {
        if (i->second.Lights.size() != 0)
        {
            for (unsigned int j = 0; j < i->second.nrLights; j++)
            {
                // NOTE: This push_back is causing error
                // when reading ugly_city.fbx, it thinks there are 88 lights when there are only 8
                lights.push_back(i->second.Lights.at(j));
            }
        }
    }
    return lights;
}

void ResourceManager::SaveMatrix(glm::mat4 matrix, std::string name)
{
    SGL_PROFILE_FUNCTION();
    Matrices[name] = matrix;
}

glm::mat4 ResourceManager::LoadMatrix(std::string name)
{
    SGL_PROFILE_FUNCTION();
    return Matrices[name];
}

void ResourceManager::SaveVec3(glm::vec3 vec3, std::string name)
{
    SGL_PROFILE_FUNCTION();
    VecThrees[name] = vec3;
}

glm::vec3 ResourceManager::LoadVec3(std::string name)
{
    SGL_PROFILE_FUNCTION();
    return VecThrees[name];
}
