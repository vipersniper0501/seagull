#ifndef MESH_HEADER_FILE
#define MESH_HEADER_FILE

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shaders.h"
#include "Utils.h"

using namespace std;


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 Color;
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};

class Mesh {
    public:
        // mesh data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
        unsigned int VAO;
        
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            SetupMesh();
        };

        void Draw(Shader &shader)
        {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                GLCall(glActiveTexture(GL_TEXTURE0 + i)); // activate proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name = textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if(name == "texture_normal")
                    number = std::to_string(normalNr++);

                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
            }

            // Draw Mesh
            GLCall(glBindVertexArray(VAO));
            GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
            GLCall(glBindVertexArray(0));

            GLCall(glActiveTexture(GL_TEXTURE0));
        }

        void Destroy()
        {
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
            GLCall(glDeleteBuffers(1, &VBO));
            
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
            GLCall(glDeleteBuffers(1, &EBO));

            GLCall(glBindVertexArray(0));
            GLCall(glDeleteVertexArrays(1, &VAO));
        }
    private:
        // render data
        unsigned int VBO, EBO;
        void SetupMesh()
        {
            GLCall(glGenVertexArrays(1, &VAO));
            GLCall(glGenBuffers(1, &VBO));
            GLCall(glGenBuffers(1, &EBO));

            GLCall(glBindVertexArray(VAO));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

            // Cant pass in &data.vertices[0] like this as struct memory is non-sequential in standard C
            // result is sending garbage values into array buffer.... I think.
            GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

            // vertex positions
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL));

            // vertex normals
            GLCall(glEnableVertexAttribArray(1));
            GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
            
            // vertex texture coords
            GLCall(glEnableVertexAttribArray(2));
            GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

            GLCall(glEnableVertexAttribArray(3));
            GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color)));

            GLCall(glBindVertexArray(0));
        }
};



// typedef struct {
    // unsigned int id;
    // unsigned int vertices_size;
    // Vertex *vertices;
    // unsigned int indices_size;
    // unsigned int *indices;
    // unsigned int textures_size;
    // Texture *textures;
// } MeshData;

// GLuint *MeshIDs;

// void SetupMesh(MeshData data);
// void DrawMesh(MeshData data, GLuint MeshID);

#endif
