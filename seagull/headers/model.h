#ifndef MODEL_HEADER_FILE
#define MODEL_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "mesh.h"

// unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model 
{
    public:
        // model data
        vector<Texture> textures_loaded;
        vector<Mesh> meshes;
        std::string directory;
        std::string FileType;

        Model(std::string const &path)
        {
            loadModel(path);
        }

        void Draw(Shader &shader)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                meshes[i].Draw(shader);
            }
        }
    private:

        void loadModel(std::string path)
        {
            Assimp::Importer import;
            const aiScene *scene = import.ReadFile(path, 
                    aiProcess_Triangulate | 
                    aiProcess_FlipUVs | 
                    aiProcess_CalcTangentSpace |
                    aiProcess_JoinIdenticalVertices);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "\nASSIMP ERROR: " << import.GetErrorString() << std::endl;
                return;
            }
            directory = path.substr(0, path.find_last_of('/'));
            FileType = std::strrchr(path.c_str(), '.');
            std::cout << "FileType: " << FileType << std::endl;

            processNode(scene->mRootNode, scene);
        }

        /*
         * Process Assimp Nodes by adding the nodes meshes to the global vector
         * list of Meshes
         */
        void processNode(aiNode *node, const aiScene *scene)
        {
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // then do the same for each of its children
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene)
        {
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;

            // Vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector;
                
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;

                vertex.Position = vector;

                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }

                // Texture Coordinates
                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;

                    //tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;

                    //Bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                if (mesh->mColors[0])
                {
                    glm::vec4 colors;

                    colors.r = mesh->mColors[0][i].r;
                    colors.g = mesh->mColors[0][i].g;
                    colors.b = mesh->mColors[0][i].b;
                    colors.a = mesh->mColors[0][i].a;

                    vertex.Color = colors;
                }

                vertices.push_back(vertex);
            }

            // Indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                {
                    indices.push_back(face.mIndices[j]);
                }
            }

            // Materials
            if (mesh->mMaterialIndex >= 0)
            {
                aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

                vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

                vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

                vector<Texture> normalMaps;
                if (strcmp(FileType.c_str(), ".obj"))
                {
                    normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
                }
                else
                {
                    normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", scene);
                }
                textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

                vector<Texture> heightMaps;
                if (strcmp(FileType.c_str(), ".obj"))
                {
                    heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
                }
                else
                {
                    heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", scene);
                }
                textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

                vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao", scene);
                textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());

            }

            return Mesh(vertices, indices, textures);
        }

        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const aiScene *scene)
        {
            vector<Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip)
                {
                    Texture texture;
                    if (auto emb_texture = scene->GetEmbeddedTexture(str.C_Str()))
                    {
                        texture.id = EmbeddedTextureFromFile(emb_texture);
                    }
                    else
                    {
                        texture.id = TextureFromFile(str.C_Str(), this->directory);
                    }
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                    std::cout << "Loaded a Material" << std::endl;
                }
            }
            return textures;
        }

        unsigned int TextureFromFile(const char *path, const string &directory)
        {
            string filename = string(path);
            filename = directory + '/' + filename;

            std::cout << "Texture Map File: " << filename << std::endl;

            unsigned int textureID;
            GLCall(glGenTextures(1, &textureID));

            int width, height, nrComponents;
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format = 0;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
                GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));

                GLCall(glGenerateMipmap(GL_TEXTURE_2D));

                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

        unsigned int EmbeddedTextureFromFile(const aiTexture *embedded_texture)
        {

            unsigned int textureID;
            GLCall(glGenTextures(1, &textureID));

            std::cout << "Embedded Texture Name: " << embedded_texture->mFilename.C_Str() << std::endl;
            unsigned char *image_data = nullptr;

            int width, height, components_per_pixel;
            if (embedded_texture->mHeight == 0)
            {
                image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embedded_texture->pcData), embedded_texture->mWidth, &width, &height, &components_per_pixel, 0);
            }
            else
            {
                image_data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embedded_texture->pcData), embedded_texture->mWidth * embedded_texture->mHeight, &width, &height, &components_per_pixel, 0);
            }

            if (image_data)
            {
                GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

                if (components_per_pixel == 3)
                {
                    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data));
                }
                else if (components_per_pixel == 4)
                {
                    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data));
                }

                GLCall(glGenerateMipmap(GL_TEXTURE_2D));

                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            }
            else
            {
                std::cout << "Error loading embedded texture from memory: " << embedded_texture->mFilename.C_Str() << std::endl;
                stbi_image_free(image_data);
            }


            return textureID;
        }

};



#endif
