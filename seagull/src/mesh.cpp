#include "mesh.h"

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int aoNr = 1;
    unsigned int emissiveNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + i)); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;

        const char *cname = textures[i].type.c_str();

        if (std::strcmp(cname, "texture_diffuse") == 0)
            number = std::to_string(diffuseNr++);
        else if (std::strcmp(cname, "texture_specular") == 0)
            number = std::to_string(specularNr++);
        else if (std::strcmp(cname, "texture_normal") == 0)
            number = std::to_string(normalNr++);
        else if (std::strcmp(cname, "texture_height") == 0)
            number = std::to_string(heightNr++);
        else if (std::strcmp(cname, "texture_ao") == 0)
            number = std::to_string(aoNr++);
        else if (std::strcmp(cname, "texture_emissive") == 0)
            number = std::to_string(emissiveNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        GLCall(glBindTexture(GL_TEXTURE_2D, textures[i].id));
    }

    // Draw Mesh
    GLCall(glBindVertexArray(VAO));
    GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    GLCall(glBindVertexArray(0));

    GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::Destroy()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &VBO));
    
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glDeleteBuffers(1, &EBO));

    GLCall(glBindVertexArray(0));
    GLCall(glDeleteVertexArrays(1, &VAO));
}

void Mesh::SetupMesh()
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
    GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

    GLCall(glEnableVertexAttribArray(4));
    GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));

    GLCall(glEnableVertexAttribArray(5));
    GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color)));

    GLCall(glBindVertexArray(0));
}
