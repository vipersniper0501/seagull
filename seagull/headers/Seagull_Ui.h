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

class UI
{
    public:

        static float lampLocation[3];

        UI()
        {
        }

        void init(GLFWwindow *window)
        {

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 400");

            ImGui::StyleColorsDark();
        }

        void NewFrame()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void RenderUi()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void Destroy()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        static void ShowMainMenuBar()
        {

            static bool live_stats = false;
            static bool lamp_config = false;

            if (live_stats)         ShowLiveStats();
            if (lamp_config)        ShowLampConfiguration(&lamp_config);

            if(ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Debug Tools"))
                {
                    if (ImGui::MenuItem("Debug Stats", NULL, &live_stats)) {}
                    if (ImGui::MenuItem("Lamp Config", NULL, &lamp_config)) {}
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
        }

    private:

        static void ShowLiveStats()
        {
            ImGui::Begin("Debug Stats");
            
            ImGui::Text("Debug Stats:");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Text("OpenGL Version:     %s\n"
                        "GLFW Version:       %s\n"
                        "Dear ImGui Version: %s\n"
                        "Assimp:             %u.%u.%u\n", 
                        glGetString(GL_VERSION), glfwGetVersionString(), ImGui::GetVersion(),
                        aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionPatch());
            ImGui::End();
        }

        static void ShowLampConfiguration(bool *p_open)
        {
            if(!ImGui::Begin("Lamp Config", p_open))
            {
                ImGui::End();
                return;
            }

            ImGui::SliderFloat3("Lamp Location", lampLocation, -10.0f, 10.0f);

            ImGui::End();
        }
};


#endif
