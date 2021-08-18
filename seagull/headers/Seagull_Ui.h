#ifndef SEAGULL_UI_HEADER
#define SEAGULL_UI_HEADER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/version.h>

#include <sys/stat.h>

#include "filesystem.h"
#include "ResourceManager.h"

class UI
{
    public:

        static float lampLocation[3];
        static float lampIntensity;


        UI()
        {
        }

        void init(GLFWwindow *window);

        void NewFrame();

        void RenderUi();

        void Destroy();

        void ShowMainMenuBar();

        void ShowLiveStats(bool *p_open);

        void ShowLampConfiguration(bool *p_open);

        void ShowSceneHeiarchy(bool *p_open);

        void ExitApp();
};


#endif
