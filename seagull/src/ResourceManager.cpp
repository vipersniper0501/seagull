#include "ResourceManager.h"


std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;

Shader ResourceManager::LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name)
{
    Shader shader_tmp(vShaderFile, fShaderFile);
    Shaders[name] = shader_tmp;
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Model ResourceManager::LoadModel(std::string ModelFileLocation, std::string name)
{
    Model model_tmp(ModelFileLocation);
    Models[name] = model_tmp;
    return Models[name];
}

Model ResourceManager::GetModel(std::string name)
{
    return Models[name];
}

unsigned int ResourceManager::GetNumberOfLights()
{
    unsigned int lights = 0;
    for(auto i = Models.begin(); i != Models.end(); i++)
    {
        lights += i->second.nrLights;
    }
    return lights;
}

std::vector<aiLight> ResourceManager::GetLights()
{
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
