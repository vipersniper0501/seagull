#ifndef MODEL_HEADER_FILE
#define MODEL_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "mesh.h"

// unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
//

// Might just want a global Loaded_Models struct instead of SceneInfo if nothing
// else is being added to SceneInfo.

class Model 
{
    public:
        // model data
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::vector<aiLight> Lights;
        unsigned int nrLights = 0;
        std::string directory;
        std::string path;
        std::string FileType;
        std::string name;

        Model() {}

        Model(std::string const &path)
        {
            loadModel(path);
        }

        void Draw(Shader &shader);
        // Not implemented yet
        //
        // Will render the same object in multiple positions
        // Useful for things such as lights.
        void DrawInstanced(Shader &shader, glm::vec3 locations);


    private:

        void loadModel(std::string path);

        /*
         * Process Assimp Nodes by adding the nodes meshes to the global vector
         * list of Meshes
         */
        void processNode(aiNode *node, const aiScene *scene);

        Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 scaling);

        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const aiScene *scene);

        unsigned int TextureFromFile(const char *path, const std::string &directory);

        unsigned int EmbeddedTextureFromFile(const aiTexture *embedded_texture);

        unsigned int LoadDefaultTexture(GLubyte color = 255);

};



#endif
