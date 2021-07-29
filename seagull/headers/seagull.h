#ifndef SEAGULL_HEADER_FILE
#define SEAGULL_HEADER_FILE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/version.h>

#include "shaders.h"
#include "mesh.h"
#include "model.h"
#include "camera.h"
#include "Utils.h"
#include "Seagull_Ui.h"


GLFWwindow* InitGlfwWindow(void);
void InitializeGL(GLFWwindow *window);
void processKeypress(GLFWwindow *window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);
#endif
