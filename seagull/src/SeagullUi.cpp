#include "SeagullUi.h"


static void ShowLiveStats(bool *p_open)
{
    ImGui::Begin("Stats");
    
    ImGui::Text("Live Stats:");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}



static void ShowMainMenuBar()
{

    static bool live_stats = false;

    if (live_stats)         ShowLiveStats(&live_stats);

    if(ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug Tools"))
        {
            if (ImGui::MenuItem("Stats", NULL, &live_stats)) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void ShowSeagullUI()
{
    ShowMainMenuBar();
}
