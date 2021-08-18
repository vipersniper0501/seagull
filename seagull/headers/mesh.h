#ifndef MESH_HEADER_FILE
#define MESH_HEADER_FILE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shaders.h"
#include "Utils.h"



struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    glm::vec4 Color;
};

struct Texture{
    unsigned int id;
    std::string type;
    std::string path;
    std::string name;
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        unsigned int VAO;
        std::string name;
        
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, std::string name = "")
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            this->name = name;

            SetupMesh();
        };

        /*
         * Binds textures and draws the mesh to the screen
         */
        void Draw(Shader &shader);

        /*
         * Unbinds Buffers (sets them to the buffer location '0') and deletes them.
         */
        void Destroy();


    private:
        // render data
        unsigned int VBO, EBO;

        void SetupMesh();
};


#endif
