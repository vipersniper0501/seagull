#include "ResourceManager.h"


std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Model> ResourceManager::Models;
SceneData SceneInfo;

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
