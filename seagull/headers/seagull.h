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
#include "Utils.h"

// ---- vbo.h stuff -----
struct Vertexs{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 Color;
};

vector<Vertex>Vertices = {
    { { 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } },
    // Top
    { { -0.2f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { 0.2f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { 0.0f, 0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // Bottom
    { { -0.2f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { 0.2f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { 0.0f, -0.8f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { 0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // Left
    { { -0.8f, -0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { -0.8f, 0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { -0.8f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { -1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
    // Right
    { { 0.8f, -0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } },
    { { 0.8f, 0.2f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { { 0.8f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 0.0f, 1.0f, 1.0f, 1.0f } },
    { { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } }
};

vector<unsigned int>Indices = {
    // Top
    0, 1, 3,
    0, 3, 2,
    3, 1, 4,
    3, 4, 2,
    // Bottom
    0, 5, 7,
    0, 7, 6,
    7, 5, 8,
    7, 8, 6,
    // Left
    0, 9, 11,
    0, 11, 10,
    11, 9, 12,
    11, 12, 10,
    // Right
    0, 13, 15,
    0, 15, 14,
    15, 13, 16,
    15, 16, 14
};
vector<Texture>textures = {

};

// ---- vbo.h stuff ends ----




void Initialize(void);
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods);

#endif
