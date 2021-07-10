#include "model.h"


// char *directory;

// void loadModel(char *path)
// {
    // const struct aiScene *scene = aiImportFile(path,
           // aiProcess_Triangulate | aiProcess_FlipUVs);

    // if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
       // fprintf(stderr, "ASSIMP IMPORT ERROR: %s\n", aiGetErrorString());
       // return;
    // }
    // char *lastSlashLoc = strrchr(path, '/');
    // char *baseName = lastSlashLoc ? lastSlashLoc + 1 : path;
    // char *dirpath = malloc(strlen(path) * sizeof(char));
    // strncpy(dirpath, path, strlen(path) - strlen(baseName));

    // directory = dirpath;
    // free(dirpath);
    // processNode(scene->mRootNode, scene);
// }

// void processNode(struct aiNode *node, const struct aiScene *scene)
// {
    // for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        // struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

    // }
// }

// MeshData processMesh(struct aiMesh *mesh, const struct aiScene *scene)
// {

// }

// void DrawModel(MeshData data)
// {
    // unsigned int meshid_size = sizeof(&MeshIDs)/sizeof(GLuint);
    // for (unsigned int i = 0; i < meshid_size; i++) {
        // DrawMesh(data, MeshIDs[i]);
    // }
// }

