#ifndef SCENE_MANAGER_HEADER_FILE
#define SCENE_MANAGER_HEADER_FILE

#include <vector>


// #include "mesh.h"
#include "model.h"


struct SceneData {
    std::vector<Model> loaded_models; 
    // std::vector<Mesh> loaded_meshes;
    // std::vector<Texture> loaded_textures;
} SceneInfo;

// class SceneManager
// {
    // static std::vector<Model> loaded_models;
    // public:
        // SceneManager();
        // ~SceneManager();

        // void addModelToScene();
        // void destroyModel();

        // void drawScene();

// };




#endif
