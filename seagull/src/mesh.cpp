#include "mesh.h"

#include <GL/glew.h>
#include <stdlib.h>



// void SetupMesh()
// {
    /* Errors are occurring here! */

    // float *verticesLoc = malloc(data.vertices_size * sizeof(float));
    // for (int j = 0; j < data.vertices_size; j++) {
        // int loc = j*9;
        // for (int p = 0; p < 5; p++) {
            // verticesLoc[loc+p] = data.vertices[j].Position[p];
        // }
        // for (int n = 0; n < 4; n++) {
            // verticesLoc[loc+4+n] = data.vertices[j].Normal[n];
        // }
        // for (int t = 0; t < 3; t++) {
            // verticesLoc[loc+4+3+t] = data.vertices[j].TexCoords[t];
        // }
    // }

    // for (int f = 0; f < data.vertices_size; f++) {
        // int loc = f*9;
        // fprintf(stdout, "VERTEX ARRAY [%d]: { %f, %f, %f, %f, %f, %f, %f, %f, %f }\n",
                         // f, verticesLoc[loc], verticesLoc[loc+1], verticesLoc[loc+2],
                         // verticesLoc[loc+3], verticesLoc[loc+4], verticesLoc[loc+5],
                         // verticesLoc[loc+6], verticesLoc[loc+7], verticesLoc[loc+8]);
    // }
    // const size_t BufferSize = sizeof(data.vertices_size);
    // const size_t IndexBufferSize = sizeof(data.indices_size);

    // GLCall(glGenVertexArrays(1, &VAO));
    // GLCall(glGenBuffers(1, &VBO));
    // GLCall(glGenBuffers(1, &EBO));

    // GLCall(glBindVertexArray(VAO));
    // GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    // // Cant pass in &data.vertices[0] like this as struct memory is non-sequential in standard C
    // // result is sending garbage values into array buffer.... I think.
    // GLCall(glBufferData(GL_ARRAY_BUFFER, BufferSize, &data.vertices[0], GL_STATIC_DRAW));

    // GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    // GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, &data.indices[0], GL_STATIC_DRAW));

    // // vertex positions
    // GLCall(glEnableVertexAttribArray(0));
    // GLCall(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL));

    // // vertex normals
    // GLCall(glEnableVertexAttribArray(1));
    // GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

    // // vertex texture coords
    // GLCall(glEnableVertexAttribArray(2));
    // GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

    // GLCall(glBindVertexArray(0));

// }

// void DrawMesh(MeshData data, GLuint MeshID)
// {
    // GLCall(glBindVertexArray(MeshIDs[MeshID]));
    // [> GLCall(glBindVertexArray(VAO)); <]
    // GLCall(glDrawElements(GL_TRIANGLES, data.indices_size, GL_UNSIGNED_INT, NULL));
    // GLCall(glBindVertexArray(0));
// }
