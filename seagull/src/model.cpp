#include "model.h"
#include "profiler.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <cmath>

void Model::Draw(Shader &shader)
{
    SGL_PROFILE_FUNCTION();
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(std::string path)
{
    SGL_PROFILE_FUNCTION();
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
    this->path = path;
    directory = path.substr(0, path.find_last_of('/'));
    FileType = std::strrchr(path.c_str(), '.');
    std::cout << "FileType: " << FileType << std::endl;
    name = path.substr(path.find_last_of('/'), path.find_last_of('.'));
    std::cout << "Model Name: " << name.c_str() << std::endl;

    if (scene->HasLights())
    {
        nrLights += scene->mNumLights;
        for (unsigned int i = 0; i < scene->mNumLights; i++)
        {
            aiLight *light = scene->mLights[i];
            /*
                Okay, so it looks like the transformations/locations of where the lights are supposed
                to be can be found in its corresponding node under the rootnode. when comparing the values to blender,
                all values have their decimal shifted to the right 2 times in the fbx format
                (looks fine for gltf).
                It is probably best to have a proccessLights function to handle this in the processNode function.
                simple if statement of if name == lightName found in lights. if so, process node as if it were a light
                and retrieve transformation/locations.

                found this information from the following link: https://github.com/assimp/assimp/issues/2381
            */
            if (FileType == ".gltf")
            {
                // This is a temporary fix
                std::string name = light->mName.C_Str();
                for (auto i = name.find("_Orientation"); i != std::string::npos; i = name.find("_Orientation"))
                    name.erase(i, 12);
                light->mName.Set(name.c_str());
            }
            Lights.push_back(*light);
        }
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    SGL_PROFILE_FUNCTION();
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        glm::vec3 s;
        if (FileType == ".fbx")
        {
            s.x = node->mTransformation.a1 * std::pow(10, 2);
            s.y = -(node->mTransformation.b2) * std::pow(10, 2);
            s.z = node->mTransformation.c3 * std::pow(10, 2);
        }
        else
        {
            s.x = node->mTransformation.a1;
            s.y = node->mTransformation.b2;
            s.z = node->mTransformation.c3;
        }
        if (round(s.x) == 0.0f)
            s.x = 1.0;
        if (round(s.y) == 0.0f)
            s.y = 1.0;
        if (round(s.z) == 0.0f)
            s.z = 1.0;
        meshes.push_back(processMesh(mesh, scene, s));
    }
    for (unsigned int i = 0; i < Lights.size(); i++)
    {
        // this can/should be optimized somehow.
        std::cout << "Node name : " << node->mName.C_Str() << std::endl;
        std::cout << "Light name : " << Lights[i].mName.C_Str() << std::endl;
        if (node->mName == Lights[i].mName)
        {
            // This currently only works for FBX files. gltf is acting kinda weird
            std::cout << "Node and Light name match !!!\n" << std::endl;
            if (FileType == ".fbx")
            {
                Lights[i].mPosition.x = node->mTransformation.a4 * std::pow(10, -2);
                Lights[i].mPosition.y = -(node->mTransformation.c4 * std::pow(10, -2));
                Lights[i].mPosition.z = node->mTransformation.b4 * std::pow(10, -2);
            }
            else
            {
                Lights[i].mPosition.x = node->mTransformation.a4;
                Lights[i].mPosition.y = -(node->mTransformation.c4);
                Lights[i].mPosition.z = node->mTransformation.b4;
            }
        }

    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 scaling)
{
    SGL_PROFILE_FUNCTION();
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    // float shinyLevel;

    // Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        
        vector.x = mesh->mVertices[i].x * scaling.x;
        vector.y = mesh->mVertices[i].y * scaling.y;
        vector.z = mesh->mVertices[i].z * scaling.z;

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

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Texture> normalMaps;
        if (FileType == ".obj")
        {
            normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
        }
        else
        {
            normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", scene);
        }
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Texture> heightMaps;
        if (FileType == ".obj")
        {
            heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
        }
        else
        {
            heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", scene);
        }
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_ao", scene);
        textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());

        std::vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive", scene);
        textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

        // Get shiny level
        // if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shinyLevel))
        // {
            // shinyLevel = 64.0f;
        // }

    }

    // std::cout << "New Mesh parsed w/ name: " << mesh->mName.C_Str() << std::endl;

    return Mesh(vertices, indices, textures, shinyLevel, (std::string)mesh->mName.C_Str());
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, const aiScene *scene)
{
    SGL_PROFILE_FUNCTION();
    std::vector<Texture> textures;
    if (mat->GetTextureCount(type) == 0)
    {
        // Note: Need to figure out why this gets run through a ton of times after
        // initial loading of textures. It shouldn't be doing that.
        //
        // It could possibly be checking for textures for every single mesh in the backpack
        // model.
        std::cout << "HEY! No texture of type " << typeName << " found!" << std::endl;

        Texture texture;
        if (typeName == "texture_emissive")
            texture.id = LoadDefaultTexture(0);
        else
            texture.id = LoadDefaultTexture();
        texture.name = "texture_missing";
        texture.type = typeName;
        textures.push_back(texture);
        return textures;
    }
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
                texture.name = emb_texture->mFilename.C_Str();
            }
            else
            {
                texture.id = TextureFromFile(str.C_Str(), this->directory);
                texture.name = str.C_Str();
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


unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
    SGL_PROFILE_FUNCTION();
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    std::cout << "Texture Map File: " << filename << std::endl;

    unsigned int textureID;
    GLCall(glGenTextures(1, &textureID));

    stbi_set_flip_vertically_on_load(true);

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

unsigned int Model::EmbeddedTextureFromFile(const aiTexture *embedded_texture)
{

    SGL_PROFILE_FUNCTION();
    unsigned int textureID;
    GLCall(glGenTextures(1, &textureID));

    std::cout << "Embedded Texture Name: " << embedded_texture->mFilename.C_Str() << std::endl;
    unsigned char *image_data = nullptr;

    stbi_set_flip_vertically_on_load(false);

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
        GLenum format = 0;
        if (components_per_pixel == 1)
            format = GL_RED;
        else if (components_per_pixel == 3)
            format = GL_RGB;
        else if (components_per_pixel == 4)
            format = GL_RGBA;

        GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, image_data));

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

unsigned int Model::LoadDefaultTexture(GLubyte color)
{
    SGL_PROFILE_FUNCTION();
    unsigned int textureID;
    GLCall(glGenTextures(1, &textureID));

    GLubyte data[] = {color, color, color, color};
    
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    return textureID;
}





