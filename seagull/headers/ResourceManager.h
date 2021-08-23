#ifndef SCENE_MANAGER_HEADER_FILE
#define SCENE_MANAGER_HEADER_FILE

#include <map>
#include <string>

// #include "mesh.h"
#include "model.h"
#include "shaders.h"



// struct SceneData {
    // std::vector<Model> loaded_models;
    // // std::vector<Mesh> loaded_meshes;
    // // std::vector<Texture> loaded_textures;
// };

class ResourceManager {
    public:
        // Temporary struct
        // static SceneData SceneInfo;

        static std::map<std::string, Model>  Models;
        static std::map<std::string, Shader> Shaders;

        static Shader LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name);
        static Shader GetShader(std::string name);

        static Model  LoadModel(std::string ModelFileLocation, std::string name);
        static Model  GetModel(std::string name);

        static unsigned int GetNumberOfLights();
        static std::vector<aiLight> GetLights();

    private:
        ResourceManager() {}

};




#endif
