#ifndef SEAGULL_HEADER_FILE
#define SEAGULL_HEADER_FILE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "mesh.h"
#include "model.h"
#include "Utils.h"
#include "SeagullUi.h"

// ---- vbo.h stuff -----
struct Vertexs{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 Color;
};

// vector<Vertex>Vertices = {
    // { { 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
    // // Top
    // { { -0.2f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    // { { 0.2f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    // { { 0.0f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    // { { 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // // Bottom
    // { { -0.2f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    // { { 0.2f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    // { { 0.0f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    // { { 0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // // Left
    // { { -0.8f, -0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    // { { -0.8f, 0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    // { { -0.8f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    // { { -1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // // Right
    // { { 0.8f, -0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    // { { 0.8f, 0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    // { { 0.8f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    // { { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },

    // { { 0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} } //, { 1.0f, 1.0f, 1.0f, 1.0f } }
// };

// vector<unsigned int>Indices = {
    // // Top
    // 0, 1, 3,
    // 0, 3, 2,
    // 3, 1, 4,
    // 3, 4, 2,
    // 17, 3, 2,
    // 17, 1, 3,
    // // Bottom
    // 0, 5, 7,
    // 0, 7, 6,
    // 7, 5, 8,
    // 7, 8, 6,
    // 17, 7, 6,
    // 17, 5, 7,
    // // Left
    // 0, 9, 11,
    // 0, 11, 10,
    // 11, 9, 12,
    // 11, 12, 10,
    // 17, 11, 10,
    // 17, 9, 11,
    // // Right
    // 0, 13, 15,
    // 0, 15, 14,
    // 15, 13, 16,
    // 15, 16, 14,
    // 17, 15, 14,
    // 17, 13, 15
// };
vector<Texture>textures = {
};

glm::vec3 crossPositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

// ---- vbo.h stuff ends ----




void Initialize(void);
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif
